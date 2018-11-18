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
    float levelInL = 0.0;
    float levelInR = 0.0;
    float levelOutL = 0.0;
    float levelOutR = 0.0;
    CompressorStatus compressor;
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
    Sample* _bufferFilter = nullptr;
    Sample* _bufferComp = nullptr;
    Filter _filter;
    Compressor _compressor;
    float _volume = 1.0;
    LevelMeter _levelMeterIn;
    LevelMeter _levelMeterOut;
};

#endif // TRACK_H
