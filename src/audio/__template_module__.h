#ifndef TEMPLATEMODULE_H
#define TEMPLATEMODULE_H

#include <cmath>
#include "shared/typedefs.h"
#include "shared/constants.h"
#include "audio/abstractmodule.h"
#include "audio/_samplemeter.h"


class TemplateModule : public AbstractModule
{
public:
    TemplateModule(const nFrame bufferSize = 0) :
        AbstractModule(bufferSize) { }
    ModuleStatus const status() override;
    void update(ModuleStatus status_) override;
    void process(Sample const * bufferIn, const nFrame time, const SampleBank * /*sampleBank*/) override;
private:
    int _a;
};


#endif // TEMPLATEMODULE_H
