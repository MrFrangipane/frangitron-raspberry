#include "djdeck.h"


void DjDeck::update(ModuleStatus status)
{
    _trackIndex = status.params[2].value;
}

const ModuleStatus DjDeck::status()
{
    ModuleStatus status_;

    if( _trackIndex != _deckInfos.audioFileIndex )
    {
        _deckInfos.audioFileIndex = _trackIndex;
        _deckInfos.needsLoading = true;
        _trackBank->setDeckInfos(_deckInfos.index, _deckInfos);
    } else {
        _deckInfos = _trackBank->deckInfos(_deckInfos.index);
    }

    status_.params[2].name = "Track";
    status_.params[2].isVisible = true;
    status_.params[2].min = 0;
    status_.params[2].max = _trackBank->audioFileCount() - 1;
    status_.params[2].value = _trackIndex;
    status_.params[2].step = 1;

    status_.params[5].name = "Position";
    status_.params[5].value = _position;

    status_.levelOut = fmax(_outMeterL.rms.average, _outMeterR.rms.average);
    status_.isClipping = _outMeterL.isClipping() || _outMeterR.isClipping();

    return status_;
}

void DjDeck::process(Sample const * bufferIn, const nFrame time)
{
    AudioFileInfos track = _trackBank->audioFileInfos(_deckInfos.audioFileIndex);
    nFrame trackPosition = (time % track.frameCount);
    nSample sampleIndex = trackPosition * track.channelCount;
    _position = (float)trackPosition / (float)track.frameCount;

    for( nFrame i = 0; i < _bufferSize; i++ )
    {
        _left = i * 2;
        _right = _left + 1;
        sampleIndex = ((trackPosition + i) % track.frameCount) * track.channelCount;

        _bufferOut[_left] = bufferIn[_left] + _trackBank->sample(_deckInfos.index, sampleIndex);
        _bufferOut[_right] = bufferIn[_right] + _trackBank->sample(_deckInfos.index, sampleIndex + 1);

        _outMeterL.stepCompute(_bufferOut[_left]);
        _outMeterR.stepCompute(_bufferOut[_right]);
    }
}

