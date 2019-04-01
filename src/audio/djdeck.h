#ifndef DJDECK_H
#define DJDECK_H

#include <cmath>
#include "shared/typedefs.h"
#include "audio/_samplebank.h"
#include "audio/abstractmodule.h"
#include "audio/_djtrackbank.h"


class DjDeck : public AbstractModule
{
public:
    DjDeck(
        DjDeckInfos deckInfos,
        const nFrame bufferSize = 0,
        DjTrackBank * const trackBank = nullptr,
        const bool isRoutedToMaster = false
    ) :
        AbstractModule(bufferSize, isRoutedToMaster),
        _deckInfos(deckInfos),
        _trackBank(trackBank)
    { }
    ModuleStatus const status() override;
    void update(ModuleStatus status_) override;
    void process(Sample const * bufferIn, const ClockStatus time) override;

private:
    DjDeckInfos _deckInfos;
    AudioFileInfos _audioFile;
    DjTrackBank * const _trackBank;
    int _audioFileIndex = -1;
    int _previousAudioFileIndex = -1;
    float _position = 0.0;
    nFrame _frameStart = 0;
    nBar _previousBar = 0;
    int _cue = 0;
    float _cueFloat = 0.0;
};

#endif // DJDECK_H
