#ifndef SAMPLEPLAYER_H
#define SAMPLEPLAYER_H

#include <cmath>
#include "shared/typedefs.h"
#include "shared/constants.h"
#include "audio/_envelope.h"
#include "audio/_samplemeter.h"
#include "audio/abstractmodule.h"


class SamplePlayer : public AbstractModule
{
public:
    SamplePlayer(const nFrame bufferSize = 0) :
        AbstractModule(bufferSize) { }
    ModuleStatus const status() override;
    void update(ModuleStatus status_) override;
    void process(Sample const * bufferIn, const nFrame time, const SampleBank * sampleBank) override;
    void gate(nFrame time) override;
private:
    float _amplitude = 0;
    _Envelope _envelope_amplitude;
    _Envelope _envelope_sidechain;
};


#endif // SAMPLEPLAYER_H
