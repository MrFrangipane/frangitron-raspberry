#ifndef RECORDER_H
#define RECORDER_H

#include <thread>
#include <chrono>
#include "shared/typedefs.h"


typedef void (*RecorderCallback)(
    void *userData
);


class Recorder
{
public:
    Recorder(nFrame /*buffer_size*/) { }
    void setCallback(RecorderCallback callback, void *userData) { _thread = std::thread(callback, userData); }
    void start() { _thread.detach(); }

private:
    std::thread _thread;
};

#endif // RECORDER_H
