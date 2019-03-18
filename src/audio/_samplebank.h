#ifndef SAMPLEBANK_H
#define SAMPLEBANK_H

#include <vector>
#include <string>
#include "shared/constants.h"
#include "shared/typedefs.h"
#include "shared/structures.h"


struct SampleInfos{
    std::string name = "";
    int channelCount = 0;
    nFrame frameCount = 0;
    nSample startSample = 0;
    std::string filepath = "";
};


class SampleBank
{
public:
    SampleBank() { }
    Sample left(int clipIndex, nFrame frame) const;
    Sample right(int clipIndex, nFrame frame) const;
    void registerAudioFile(AudioFileInfos audioFileInfos);
    Sample * pointerToSample(SampleInfos sampleInfos, bool updateProgress=true);
    void incrementSample(nFrame value) { _samplesLoaded += value; }
    int loadingProgress() { return _samplesLoaded * 100 / _samples.size(); }
    const std::vector<SampleInfos> & registeredSamples() { return _samplesInfos; }
    nSample currentSample() { return _samplesLoaded; }

private:
    std::vector<Sample> _samples;
    std::vector<SampleInfos> _samplesInfos;
    nSample _samplesLoaded = 0;
};

#endif // SAMPLEBANK_H
