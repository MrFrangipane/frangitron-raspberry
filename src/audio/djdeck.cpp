#include "djdeck.h"


void DjDeck::update(ModuleStatus status)
{
    _audioFileIndex = status.params[2].value;
}

const ModuleStatus DjDeck::status()
{
    ModuleStatus status_;

    if( _audioFileIndex != _deckInfos.audioFileIndex )
    {
        _deckInfos.audioFileIndex = _audioFileIndex;
        _deckInfos.needsLoading = true;
        _trackBank->setDeckInfos(_deckInfos.index, _deckInfos);
    } else {
        _deckInfos = _trackBank->deckInfos(_deckInfos.index);
    }

    status_.params[2].name = "Track";
    status_.params[2].isVisible = true;
    status_.params[2].min = -1;
    status_.params[2].max = _trackBank->audioFileCount() - 1;
    status_.params[2].value = _audioFileIndex;
    status_.params[2].step = 1;

    status_.params[5].name = "Position";
    status_.params[5].value = _position;

    status_.levelOut = fmax(_outMeterL.rms.average, _outMeterR.rms.average);
    status_.isClipping = _outMeterL.isClipping() || _outMeterR.isClipping();

    return status_;
}

void DjDeck::process(Sample const * bufferIn, const ClockStatus time)
{
    bool noFileSelected = _deckInfos.audioFileIndex < 0;
    bool fileChanged = _previousAudioFileIndex != _audioFileIndex;

    if( fileChanged )
        _previousAudioFileIndex = -2; // Ensure file changed until next bar

    // NO OUTPUT
    if( noFileSelected || (fileChanged && !time.isAtBar) || !time.isPlaying ) {
        for( nFrame i = 0; i < _bufferSize * CHANNEL_COUNT; i++ )
            _bufferOut[i] = bufferIn[i];
        _position = 0.0;
        return;
    }

    // NEW FILE
    if( fileChanged && time.isAtBar )
    {
        _previousAudioFileIndex = _audioFileIndex;
        _frameStart = time.lastBarFrame;
    }

    // FILE READ
    AudioFileInfos audioFile = _trackBank->audioFileInfos(_deckInfos.audioFileIndex);
    nFrame elapsed = time.frame - _frameStart;
    nFrame lasting = audioFile.frameCount - elapsed;
    nFrame audioFileFrame = elapsed % audioFile.frameCount;
    nSample audioFileSample = 0;

    // REALIGN
    if( time.isAtBar && (lasting < time.framePerBar / 16 || elapsed < time.framePerBar / 16) )
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
