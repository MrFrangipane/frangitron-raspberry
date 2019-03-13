#ifndef RECORDER_H
#define RECORDER_H

#include <thread>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <iostream>
#include "sndfile.hh"
#include "shared/constants.h"
#include "shared/typedefs.h"


class Recorder
{
public:
    Recorder(nFrame bufferSize, int cacheBufferCount);
    void start();
    void write(Sample * buffer);
    bool isRunning() { return _isRunning; }
    void stop() { _isRunning = false; }
    std::string filepath() { return _filepath; }
    nFrame writeIndex() { return _writeIndex; }
    nFrame bufferSize() { return _bufferSize; }
    Sample const * cacheData() { return _cache.data(); }
    Sample cache(nFrame index) { return _cache[index % _cacheSize]; }


private:
    static void mainLoop(Recorder *recorder);
    std::thread _thread;
    Buffer _cache;
    nFrame _bufferSize = 0;
    nFrame _writeIndex = 0;
    int _cacheSize = 0;
    std::string _filepath = "";
    SndfileHandle _outputFile;
    bool _isRunning = false;
};

#endif // RECORDER_H
