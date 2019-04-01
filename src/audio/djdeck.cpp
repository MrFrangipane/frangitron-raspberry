#include "djdeck.h"


void DjDeck::update(ModuleStatus status)
{
    _audioFileIndex = status.params[2].value;

    _cueFloat = status.params[3].value;
    _cue = _cueFloat / DECK_ENCODER_REGION_RATIO;
}

const ModuleStatus DjDeck::status()
{
    ModuleStatus status_;

    if( _audioFileIndex != _deckInfos.audioFileIndex )
    {
        _deckInfos.audioFileIndex = _audioFileIndex;
        _deckInfos.needsLoading = true;
        _trackBank->setDeckInfos(_deckInfos.index, _deckInfos);

        _cue = 0;
        _cueFloat = 0.0;

        if( _audioFileIndex > -1 )
            _audioFile = _trackBank->audioFileInfos(_audioFileIndex);
        else
            _audioFile = AudioFileInfos();
    } else {
        _deckInfos = _trackBank->deckInfos(_deckInfos.index);
    }

    status_.params[2].name = "Track";
    status_.params[2].isVisible = true;
    status_.params[2].min = -1;
    status_.params[2].max = _trackBank->audioFileCount() - 1;
    status_.params[2].value = _audioFileIndex;
    status_.params[2].step = 1;

    status_.params[3].name = "Cue";
    status_.params[3].isVisible = true;
    status_.params[3].min = 0;
    status_.params[3].max = (_audioFile.cueCount - 1) * DECK_ENCODER_REGION_RATIO;
    status_.params[3].value = _cueFloat;
    status_.params[3].step = 1;

    status_.params[5].name = "Position";
    status_.params[5].value = _position;

    status_.levelOut = fmax(_outMeterL.rms.average, _outMeterR.rms.average);
    status_.isClipping = _outMeterL.isClipping() || _outMeterR.isClipping();

    return status_;
}

void DjDeck::process(Sample const * bufferIn, const ClockStatus time)
{
    nFrame regionStart = _audioFile.cues[_cue].position;
    nFrame regionEnd = _audioFile.cues[_cue + 1].position + 1;

    bool noFileSelected = _deckInfos.audioFileIndex < 0;
    bool hasFileChanged = _previousAudioFileIndex != _audioFileIndex;
    bool hasBarChanged = _previousBar != time.bar;

    // NO OUTPUT
    if( noFileSelected || !time.isPlaying || (hasFileChanged && !hasBarChanged) ) {
        for( nFrame i = 0; i < _bufferSize * CHANNEL_COUNT; i++ )
            _bufferOut[i] = bufferIn[i];
        _position = regionStart;
        _audioFileFrame = regionStart;
        return;
    }

    // NEW FILE
    if( hasFileChanged && hasBarChanged )
    {
        _previousAudioFileIndex = _audioFileIndex;
        _audioFileFrame = regionStart;
    }

    if( hasBarChanged )
    {
        _previousBar = time.bar;
    }

    // FILE READ
    _position = (float)_audioFileFrame / (float)_audioFile.frameCount;

    for( nFrame i = 0; i < _bufferSize; i++ )
    {
        _left = i * 2;
        _right = _left + 1;

        _audioFileFrame += 1;
        if( _audioFileFrame == regionEnd)
            _audioFileFrame = regionStart;

        if( _audioFileFrame == _audioFile.frameCount)
            _audioFileFrame = 0;

        _bufferOut[_left] = bufferIn[_left] + _trackBank->sample(_deckInfos.index, _audioFileFrame * _audioFile.channelCount);
        _bufferOut[_right] = bufferIn[_right] + _trackBank->sample(_deckInfos.index, _audioFileFrame * _audioFile.channelCount + 1);

        _outMeterL.stepCompute(_bufferOut[_left]);
        _outMeterR.stepCompute(_bufferOut[_right]);
    }
}
