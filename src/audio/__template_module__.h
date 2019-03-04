#ifndef KICKSYNTH_H
#define KICKSYNTH_H

#include <cmath>
#include "shared/typedefs.h"
#include "audio/abstractmodule.h"
#include "audio/_samplemeter.h"


class KickSynth : public AbstractModule
{
public:
    KickSynth(const nFrame bufferSize = 0) :
        AbstractModule(bufferSize) { }
    ModuleStatus const status() override;
    void update(ModuleStatus status_) override;
    void process(Sample const * bufferIn, const nFrame /*time*/) override;
private:
    int _a;
};


#endif // KICKSYNTH_H
