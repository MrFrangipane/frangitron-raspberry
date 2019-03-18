#include "_samplebank.h"

void SampleBank::registerAudioFile(AudioFileInfos audioFileInfos)
{
    SampleInfos sampleInfos;
    sampleInfos.name = audioFileInfos.name;
    sampleInfos.filepath = audioFileInfos.filepath;
    sampleInfos.frameCount = audioFileInfos.frameCount;
    sampleInfos.channelCount = audioFileInfos.channelCount;
    sampleInfos.startSample = _samples.size();

    _samplesInfos.push_back(sampleInfos);
    _samples.resize(_samples.size() + sampleInfos.frameCount * sampleInfos.channelCount);
}

Sample * SampleBank::pointerToSample(SampleInfos sampleInfos, bool updateProgress)
{
    if( updateProgress )
        _samplesLoaded = sampleInfos.startSample;

    return (Sample *)(_samples.data() + sampleInfos.startSample);
}

Sample SampleBank::left(int clipIndex, nFrame frame) const
{
    nFrame posF = frame % _samplesInfos[clipIndex].frameCount;

    return _samples[_samplesInfos[clipIndex].startSample + posF];
}

Sample SampleBank::right(int clipIndex, nFrame frame) const
{
    nFrame posF = frame % _samplesInfos[clipIndex].frameCount;
    nSample channelOffset = _samplesInfos[clipIndex].channelCount - 1;

    return _samples[_samplesInfos[clipIndex].startSample + posF + channelOffset];
}
