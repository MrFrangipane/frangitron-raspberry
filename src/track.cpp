#include "track.h"

Track::Track()
{
    // Init Swap buffers
    _bufferFilter = new Sample[1]();
    _bufferComp = new Sample[1]();
}

Track::Track(const nFrame bufferSize) : _bufferSize(bufferSize)
{
    // Init Swap buffers
    _bufferFilter = new Sample[_bufferSize * 2]();
    _bufferComp = new Sample[_bufferSize * 2]();

    // Init Effects
    filter = Filter(_bufferSize);
    compressor = Compressor(_bufferSize);
}

TrackStatus Track::status() {
    TrackStatus status;

    status.filterFreq = filter.freq;
    status.compressorLevel = compressor.level;
    status.compressorThreshold = compressor.threshold;
    status.compressorAttack = compressor.attack;
    status.compressorRelease = compressor.release;
    status.levelInL = levelMeterIn.rmsL;
    status.levelInR = levelMeterIn.rmsR;
    status.levelOutL = levelMeterOut.rmsL;
    status.levelOutR = levelMeterOut.rmsR;

    return status;
}

void Track::setStatus(const TrackStatus status) {
    filter.freq = status.filterFreq;
    compressor.threshold = status.compressorThreshold;
    compressor.attack = status.compressorAttack;
    compressor.release = status.compressorRelease;
}

void Track::process(Sample const * bufferIn, Sample * bufferOut, const nFrame time)
{
    // Process Effects
    filter.process(bufferIn, _bufferFilter, time);
    compressor.process(_bufferFilter, _bufferComp, time);

    // Level Meter + Copy to output buffer
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
