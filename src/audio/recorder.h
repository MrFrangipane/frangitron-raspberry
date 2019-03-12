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
    Recorder(nFrame buffer_size, int cache_buffer_count);
    void start();
    void write(Sample * buffer);
    bool running() { return _running; }
    void stop() { _running = false; }
    std::string filepath() { return _filepath; }
    nFrame write_index() { return _write_index; }
    nFrame buffer_size() { return _buffer_size; }
    Sample const * cache_data() { return _cache.data(); }
    Sample cache(nFrame index) { return _cache[index % _cache_size]; }


private:
    static void main_loop(Recorder *recorder);
    std::thread _thread;
    Buffer _cache;
    nFrame _buffer_size = 0;
    nFrame _write_index = 0;
    int _cache_size = 0;
    std::string _filepath = "";
    SndfileHandle _output_file;
    bool _running = false;
};

#endif // RECORDER_H
