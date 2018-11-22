#include "track.h"

Track::Track()
{
    update(TrackStatus());
}

Track::Track(const nFrame bufferSize) : _bufferSize(bufferSize)
{
    _bufferFilter.reserve(_bufferSize * 2);
    _bufferComp.reserve(_bufferSize * 2);

    _filter = Filter(_bufferSize, FilterMode::HIGHPASS);
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

    status_.filter = _filter.status();

    return status_;
}

void Track::update(const TrackStatus status_) {
    _volume = status_.volume;
    _compressor.update(status_.compressor);
    _filter.update(status_.filter);
}

void Track::process(Sample const * bufferIn, Sample * bufferOut, const nFrame time)
{
    // EFFECTS
    _filter.process(bufferIn, _bufferFilter.data(), time);
    _compressor.process(_bufferFilter.data(), _bufferComp.data(), time);

    // LEVELS + OUTPUT
    _time = time;
    for( nFrame i = 0; i < _bufferSize; i++ ) {
        _left = i * 2;
        _right = _left + 1;

        bufferOut[_left] = _bufferComp.data()[_left];
        bufferOut[_right] = _bufferComp.data()[_right];

        _levelMeterIn.stepComputations(bufferIn[_left], bufferIn[_right]);
        _levelMeterOut.stepComputations(bufferOut[_left], bufferOut[_right]);

        _time++;
    }
}
