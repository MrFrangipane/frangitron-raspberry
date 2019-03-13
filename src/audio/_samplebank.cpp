#include "_samplebank.h"

void SampleBank::registerClip(AudioClipRegistration clip)
{
    clip.startSample = _samples.size();
    _register.push_back(clip);
    _samples.resize(_samples.size() + clip.frameCount * clip.channelCount);
}

Sample * SampleBank::pointerToSample(AudioClipRegistration clip, nFrame frameOffset, bool updateProgress)
{
    int sampleIndex = clip.startSample + frameOffset * clip.channelCount;

    if( updateProgress )
        _samplesLoaded = sampleIndex;

    return (Sample*)(_samples.data() + sampleIndex);
}

Sample SampleBank::left(int clipIndex, nFrame frame) const
{
    nFrame posF = frame % _register[clipIndex].frameCount;

    if( _register[clipIndex].channelCount == 1)
        return _samples[_register[clipIndex].startSample + posF];

    return _samples[_register[clipIndex].startSample + posF];
}

Sample SampleBank::right(int clipIndex, nFrame frame) const
{
    nFrame posF = frame % _register[clipIndex].frameCount;

    if( _register[clipIndex].channelCount == 1)
        return _samples[_register[clipIndex].startSample + posF];

    return _samples[_register[clipIndex].startSample + posF + 1];
}
