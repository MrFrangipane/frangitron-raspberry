#include "track.h"

Track::Track()
{
    _bufferFilter = new Sample[1]();
    _bufferComp = new Sample[1]();

    update(TrackStatus());
}

Track::Track(const nFrame bufferSize) : _bufferSize(bufferSize)
{
    _bufferFilter = new Sample[_bufferSize * 2]();
    _bufferComp = new Sample[_bufferSize * 2]();

    _filter = Filter(_bufferSize);
    _compressor = Compressor(_bufferSize);

    update(TrackStatus());
}

TrackStatus Track::status() {
    TrackStatus status_;

    status_.volume = _volume;
    status_.levelInL = _levelMeterIn.rmsL;
    status_.levelInR = _levelMeterIn.rmsR;
    status_.levelOutL = _levelMeterOut.rmsL;
    status_.levelOutR = _levelMeterOut.rmsR;

    status_.compressor = _compressor.status();

    return status_;
}

void Track::update(const TrackStatus status_) {
    _volume = status_.volume;
    _compressor.update(status_.compressor);
}

void Track::process(Sample const * bufferIn, Sample * bufferOut, const nFrame time)
{
    // EFFECTS
    _filter.process(bufferIn, _bufferFilter, time);
    _compressor.process(_bufferFilter, _bufferComp, time);

    // LEVELS + OUTPUT
    _time = time;
    for( nFrame i = 0; i < _bufferSize; i++ ) {
        _left = i * 2;
        _right = _left + 1;

        bufferOut[_left] = _bufferComp[_left];
        bufferOut[_right] = _bufferComp[_right];

        _levelMeterIn.stepComputations(bufferIn[_left], bufferIn[_right]);
        _levelMeterOut.stepComputations(bufferOut[_left], bufferOut[_right]);

        _time++;
    }
}
