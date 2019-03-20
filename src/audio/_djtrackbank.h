#ifndef _DJTRACKBANK_H
#define _DJTRACKBANK_H

#include <ctime>
#include <math.h>
#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include "sndfile.hh"
#include "shared/constants.h"
#include "shared/typedefs.h"
#include "shared/structures.h"
#include "shared/configuration.h"


struct DjDeckInfos {
    std::string name = "";
    bool needsLoading = false;
    int trackIndex = 0;
    int moduleIndex = 0;
};


class DjTrackBank
{
public:
    DjTrackBank();
    void registerAudioFile(AudioFileInfos djTrack);
    void registerDjDeck(DjDeckInfos djDeckInfos);
    void start();
    int trackCount() { return _tracks.size(); }
    int deckCount() { return _decks.size(); }
    void setDeckInfos(int deckIndex, DjDeckInfos infos) { _decks[deckIndex] = infos; }
    bool isRunning() { return _isRunning; }
    DjDeckInfos deckInfos(int deckIndex) { return _decks.at(deckIndex); }
    AudioFileInfos trackInfos(int trackIndex) { return _tracks.at(trackIndex); }
    Sample sample(int deckIndex, int sampleIndex);
    Sample * pointerToSample(int deckIndex) { return _samples.at(deckIndex).data(); }
    Sample peak(int deckIndex, int peakIndex) { return _peaks.at(deckIndex).at(peakIndex); }

private:
    static void mainLoop(DjTrackBank * trackBank);
    std::thread _thread;
    bool _isRunning = false;
    std::vector<AudioFileInfos> _tracks;
    std::vector<DjDeckInfos> _decks;
    std::vector<Buffer> _samples;
    std::vector<std::vector<Sample>> _peaks;
};

#endif // _DJTRACKBANK_H
