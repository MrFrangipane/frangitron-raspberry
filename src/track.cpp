#include "track.h"

Track::Track()
{
    update(TrackStatus());
}

Track::Track(const nFrame bufferSize) : _bufferSize(bufferSize)
{
    _filter = Filter(_bufferSize, FilterMode::HIGHPASS);
    _compressor = Compressor(_bufferSize);

    _bufferMaster.reserve(bufferSize * 2);

    update(TrackStatus());
}

TrackStatus Track::status() {
    TrackStatus status_;

    if( status_.compressor == nullptr ) return status_;

    status_.volume = _volume;
    *status_.levelIn = _levelMeterIn.status();
    *status_.compressor = _compressor.status();
    *status_.filter = _filter.status();

    return status_;
}

void Track::update(const TrackStatus status_) {
    _volume = status_.volume;
    if( status_.compressor != nullptr ) _compressor.update(*status_.compressor);
    if( status_.filter != nullptr ) _filter.update(*status_.filter);
}

void Track::process(Sample const * bufferIn, Sample * bufferOut, const nFrame time)
{
    // EFFECTS
    _filter.process(bufferIn, time);
    _compressor.process(_filter.bufferOut(), time);

    // LEVELS + OUTPUT
    _time = time;
    for( nFrame i = 0; i < _bufferSize; i++ ) {
        _left = i * 2;
        _right = _left + 1;

        bufferOut[_left] = _compressor.bufferOut()[_left];
        bufferOut[_right] = _compressor.bufferOut()[_right];

        _levelMeterIn.stepComputations(bufferIn[_left], bufferIn[_right]);
        _levelMeterOut.stepComputations(bufferOut[_left], bufferOut[_right]);

        _time++;
    }
}
