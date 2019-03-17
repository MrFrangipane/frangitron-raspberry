#ifndef _DJTRACKBANK_H
#define _DJTRACKBANK_H

#include <ctime>
#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <iostream>
#include "sndfile.hh"
#include "shared/configuration.h"
#include "shared/typedefs.h"


struct DjDeckInfos {
    std::string name = "";
    bool needsLoading = false;
    int trackIndex = 0;
    Sample * startPointer = nullptr;
};


class DjTrackBank
{
public:
    DjTrackBank();
    void registerDjTrack(ConfAudioFile configDjTrack);
    void registerDjDeck(DjDeckInfos djDeckInfos);
    void start();
    int trackCount() { return _tracks.size(); }
    int deckCount() { return _decks.size(); }
    void setDeckInfos(int deckIndex, DjDeckInfos infos) { _decks[deckIndex] = infos; }
    bool isRunning() { return _isRunning; }
    DjDeckInfos deckInfos(int deckIndex) { return _decks.at(deckIndex); }
    ConfAudioFile trackInfos(int trackIndex) { return _tracks.at(trackIndex); }
    Sample sample(int deckIndex, int sampleIndex);

private:
    static void mainLoop(DjTrackBank * trackBank);
    std::thread _thread;
    bool _isRunning = false;
    std::vector<ConfAudioFile> _tracks;
    std::vector<DjDeckInfos> _decks;
    std::vector<Buffer> _samples;
};

#endif // _DJTRACKBANK_H
