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
    int index = 0;
    std::string name = "";
    bool needsLoading = false;
    int audioFileIndex = 0;
    int moduleIndex = 0;
};


class DjTrackBank
{
public:
    DjTrackBank();
    void registerAudioFile(AudioFileInfos audioFile);
    DjDeckInfos registerDjDeck(DjDeckInfos djDeckInfos);
    void start();
    int trackCount() { return _tracksInfos.size(); }
    int deckCount() { return _decks.size(); }
    void setDeckInfos(int deckIndex, DjDeckInfos infos) { _decks[deckIndex] = infos; }
    bool isRunning() { return _isRunning; }
    DjDeckInfos deckInfos(int deckIndex) { return _decks.at(deckIndex); }
    AudioFileInfos audioFileInfos(int trackIndex) { return _tracksInfos.at(trackIndex); }
    Sample sample(int deckIndex, int sampleIndex);
    Sample peak(int trackIndex, int peakIndex) { return _peaks.at(trackIndex).at(peakIndex); }

private:
    static void _mainLoop(DjTrackBank * trackBank);
    Sample * _pointerToDeckData(int deckIndex) { return _decksData.at(deckIndex).data(); }
    std::thread _thread;
    bool _isRunning = false;
    std::vector<AudioFileInfos> _tracksInfos;
    std::vector<DjDeckInfos> _decks;
    std::vector<Buffer> _decksData;
    std::vector<Buffer> _peaks;
};

#endif // _DJTRACKBANK_H
