#include "_samplebank.h"

Sample SampleBank::left(int clipIndex, nFrame frame) const
{
    nFrame posF = frame % _registered[clipIndex].frameCount;

    if( _registered[clipIndex].channelCount == 1)
        return _samples[_registered[clipIndex].startSample + posF];

    return _samples[_registered[clipIndex].startSample + posF];
}

Sample SampleBank::right(int clipIndex, nFrame frame) const
{
    nFrame posF = frame % _registered[clipIndex].frameCount;

    if( _registered[clipIndex].channelCount == 1)
        return _samples[_registered[clipIndex].startSample + posF];

    return _samples[_registered[clipIndex].startSample + posF + 1];
}
