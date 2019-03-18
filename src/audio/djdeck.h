#ifndef DJDECK_H
#define DJDECK_H

#include <cmath>
#include "shared/typedefs.h"
#include "audio/_samplebank.h"
#include "audio/abstractmodule.h"
#include "audio/_djtrackbank.h"


class DjDeck : public AbstractModule
{
public:
    DjDeck(DjDeckInfos deckInfos, const nFrame bufferSize = 0) :
        AbstractModule(bufferSize),
        _deckInfos(deckInfos)
    { }
    ModuleStatus const status() override;
    void update(ModuleStatus status_) override;
    void process(Sample const * bufferIn, const nFrame time, const SampleBank * /*sampleBank*/) override;

private:
    DjDeckInfos _deckInfos;
};

#endif // DJDECK_H
