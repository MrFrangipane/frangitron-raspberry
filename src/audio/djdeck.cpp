#include "djdeck.h"


void DjDeck::update(ModuleStatus status)
{
    _trackIndex = status.params[2].value;
}

const ModuleStatus DjDeck::status()
{
    ModuleStatus status_;

    if( _trackIndex != _deckInfos.trackIndex )
    {
        _deckInfos.trackIndex = _trackIndex;
        _deckInfos.needsLoading = true;
        _trackBank->setDeckInfos(_deckInfos.index, _deckInfos);
    }

    status_.params[2].name = "Track";
    status_.params[2].isVisible = true;
    status_.params[2].min = 0;
    status_.params[2].max = _trackBank->trackCount() - 1;
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
    AudioFileInfos track = _trackBank->trackInfos(_deckInfos.trackIndex);
    nFrame trackPosition = (time % track.frameCount);
    _position = (float)trackPosition / (float)track.frameCount;

    for( nFrame i = 0; i < _bufferSize; i++ )
    {
        _left = i * 2;
        _right = _left + 1;
        trackPosition = ((trackPosition + i) % track.frameCount) * track.channelCount;

        _bufferOut[_left] = bufferIn[_left] + _trackBank->sample(_deckInfos.index, trackPosition);
        _bufferOut[_right] = bufferIn[_right] + _trackBank->sample(_deckInfos.index, trackPosition + 1);

        _outMeterL.stepCompute(_bufferOut[_left]);
        _outMeterR.stepCompute(_bufferOut[_right]);
    }
}

