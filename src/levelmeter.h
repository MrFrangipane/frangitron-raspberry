#ifndef LEVELMETER_H
#define LEVELMETER_H

#include "cmath"
#include "typedefs.h"


class LevelMeter
{
public:
    LevelMeter() {}
    float rmsL = 0.0;
    float rmsR = 0.0;
    float rmsInstantL = 0.0;
    float rmsInstantR = 0.0;
    void stepComputations(Sample left, Sample right);
private:
    void _endComputations();
    int _nStep = 0;
    int _nAverage = 0;
    float _instantL = 0.0;
    float _instantR = 0.0;
    float _averageL = 0.0;
    float _averageR = 0.0;
};

#endif // LEVELMETER_H
