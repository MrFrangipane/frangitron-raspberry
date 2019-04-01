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
    status_.params[3].max = (_audioFile.cueCount - 1) * 2;
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
    bool noFileSelected = _deckInfos.audioFileIndex < 0;
    bool hasFileChanged = _previousAudioFileIndex != _audioFileIndex;
    bool hasBarChanged = _previousBar < time.bar;

    if( hasFileChanged )
        _previousAudioFileIndex = -2; // Ensure file changed until next bar

    if( hasBarChanged )
        _previousBar = time.bar;

    // NO OUTPUT
    if( noFileSelected || (hasFileChanged && hasBarChanged) || !time.isPlaying ) {
        for( nFrame i = 0; i < _bufferSize * CHANNEL_COUNT; i++ )
            _bufferOut[i] = bufferIn[i];
        _position = 0.0;
        return;
    }

    // NEW FILE
    if( hasFileChanged && hasBarChanged )
    {
        _previousAudioFileIndex = _audioFileIndex;
        _frameStart = time.barAsFrame;
    }

    // FILE READ
    AudioFileInfos audioFile = _trackBank->audioFileInfos(_deckInfos.audioFileIndex);
    nFrame elapsed = time.frame - _frameStart;
    nFrame lasting = audioFile.frameCount - elapsed;
    nFrame audioFileFrame = elapsed % audioFile.frameCount;
    nSample audioFileSample = 0;

    // REALIGN
    if( hasBarChanged && (lasting < time.framePerBar / 16 || elapsed < time.framePerBar / 16) )
    {
        audioFileFrame = 0;
        _frameStart = time.frame;
    }

    _position = (float)audioFileFrame / (float)audioFile.frameCount;

    for( nFrame i = 0; i < _bufferSize; i++ )
    {
        _left = i * 2;
        _right = _left + 1;
        audioFileSample = ((audioFileFrame + i) % audioFile.frameCount) * audioFile.channelCount;

        _bufferOut[_left] = bufferIn[_left] + _trackBank->sample(_deckInfos.index, audioFileSample);
        _bufferOut[_right] = bufferIn[_right] + _trackBank->sample(_deckInfos.index, audioFileSample + 1);

        _outMeterL.stepCompute(_bufferOut[_left]);
        _outMeterR.stepCompute(_bufferOut[_right]);
    }
}
