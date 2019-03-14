#include "_samplebank.h"

void SampleBank::registerClip(AudioClipRegistration clip)
{
    clip.startSample = _samples.size();
    _clips.push_back(clip);
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
    nFrame posF = frame % _clips[clipIndex].frameCount;

    if( _clips[clipIndex].channelCount == 1)
        return _samples[_clips[clipIndex].startSample + posF];

    return _samples[_clips[clipIndex].startSample + posF];
}

Sample SampleBank::right(int clipIndex, nFrame frame) const
{
    nFrame posF = frame % _clips[clipIndex].frameCount;

    if( _clips[clipIndex].channelCount == 1)
        return _samples[_clips[clipIndex].startSample + posF];

    return _samples[_clips[clipIndex].startSample + posF + 1];
}
