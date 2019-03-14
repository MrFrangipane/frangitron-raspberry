#ifndef SAMPLEBANK_H
#define SAMPLEBANK_H

#include <vector>
#include <string>
#include "shared/typedefs.h"


struct AudioClipRegistration{
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
    void registerClip(AudioClipRegistration registration);
    Sample * pointerToSample(AudioClipRegistration clip, nFrame frameOffset, bool updateProgress=true);
    void incrementSample(nFrame value) { _samplesLoaded += value; }
    int loadingProgress() { return _samplesLoaded * 100 / _samples.size(); }
    const std::vector<AudioClipRegistration> & registeredClips() { return _clips; }
    nSample currentSample() { return _samplesLoaded; }

private:
    std::vector<Sample> _samples;
    std::vector<AudioClipRegistration> _clips;
    nSample _samplesLoaded = 0;
};

#endif // SAMPLEBANK_H
