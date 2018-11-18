#include "track.h"

Track::Track()
{
    // INIT BUFFERS
    _bufferFilter = new Sample[1]();
    _bufferComp = new Sample[1]();
}

Track::Track(const nFrame bufferSize) : _bufferSize(bufferSize)
{
    // INIT BUFFERS
    _bufferFilter = new Sample[_bufferSize * 2]();
    _bufferComp = new Sample[_bufferSize * 2]();

    // INIT EFFECTS
    filter = Filter(_bufferSize);
    compressor = Compressor(_bufferSize);
}

TrackStatus Track::status() {
    TrackStatus status_;

    status_.volume = volume;
    status_.levelInL = levelMeterIn.rmsL;
    status_.levelInR = levelMeterIn.rmsR;
    status_.levelOutL = levelMeterOut.rmsL;
    status_.levelOutR = levelMeterOut.rmsR;

    status_.compressor = compressor.status();

    return status_;
}

void Track::setStatus(const TrackStatus status_) {
    volume = status_.volume;
}

void Track::process(Sample const * bufferIn, Sample * bufferOut, const nFrame time)
{
    // EFFECTS
    filter.process(bufferIn, _bufferFilter, time);
    compressor.process(_bufferFilter, _bufferComp, time);

    // LEVELS + OUTPUT
    _time = time;
    for( nFrame i = 0; i < _bufferSize; i++ ) {
        _left = i * 2;
        _right = _left + 1;

        bufferOut[_left] = _bufferComp[_left];
        bufferOut[_right] = _bufferComp[_right];

        levelMeterIn.stepComputations(bufferIn[_left], bufferIn[_right]);
        levelMeterOut.stepComputations(bufferOut[_left], bufferOut[_right]);

        _time++;
    }
}
