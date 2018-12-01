#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <cmath>
#include "audio/abstractmodule.h"
#include "audio/levelmeter.h"

struct CompressorStatus {
    float rms = 0.0;
    float level = 0.0;
    bool gate = false;
    float threshold = -50.0;
    float attack = 0.2;
    float release = 0.5;
    float ratio = 100.0;
    float gain = 1.5;
};

class Compressor : public AbstractModule<CompressorStatus>
{
public:
    Compressor(const nFrame bufferSize = 0) : AbstractModule<CompressorStatus>(bufferSize) { }
    CompressorStatus status() override;
    void update(CompressorStatus status_) override;
    void process(Sample const * bufferIn, const nFrame /*time*/) override;
private:
    nFrame _nCycles = 0;
    nFrame _time = 0;
    nFrame _nLerp = 0;
    float _threshold = 0.0;
    float _attack = 0.0;
    float _release = 0.0;
    float _ratio = 0.0;
    float _gain = 0.0;
    float _level = 0.0;
    float _levelPrevious = 0.0;
    float _levelTarget = 0.0;
    float _rmsMono = 0.0;
    bool _gate = false;
    LevelMeter _levelMeter;
};

#endif // COMPRESSOR_H
