#ifndef TRACK_H
#define TRACK_H

#include <cstdint>
#include "typedefs.h"
#include "levelmeter.h"
#include "effects/filter.h"
#include "effects/compressor.h"


struct TrackStatus
{
    float volume = 1.0;
    LevelMeterStatus* levelIn = nullptr;
    CompressorStatus* compressor = nullptr;
    FilterStatus* filter = nullptr;
};


class Track
{
public:
    Track();
    Track(const nFrame bufferSize);
    TrackStatus status();
    void update(const TrackStatus status);
    void process(const Sample * bufferIn, Sample * bufferOut, const nFrame time);
private:
    nFrame _bufferSize = 0;
    nFrame _left = 0;
    nFrame _right = 0;
    nFrame _time = 0;
    Buffer _bufferMaster;
    float _volume = 1.0;
    LevelMeter _levelMeterIn;
    LevelMeter _levelMeterOut;
    Filter _filter;
    Compressor _compressor;
};

#endif // TRACK_H
