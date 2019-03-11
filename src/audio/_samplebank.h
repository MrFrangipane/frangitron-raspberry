#ifndef SAMPLEBANK_H
#define SAMPLEBANK_H

#include "shared/typedefs.h"


class SampleBank
{
public:
    SampleBank();
    Sample left(int /*clipIndex*/, nFrame /*frame*/) const { return 0.0; }
    Sample right(int /*clipIndex*/, nFrame /*frame*/) const { return 0.0; }
};

#endif // SAMPLEBANK_H
