#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

#include "typedefs.h"
#include <iostream>

class AbstractModule
{
public:
    AbstractModule(const nFrame bufferSize = 0) :
        _bufferSize(bufferSize)
    {
        _bufferOut.reserve(bufferSize * 2);
    }
    virtual Status const status() { return Status(); std::cout << "JEAN"; }
    virtual void update(Status /*status_*/) { }
    virtual void process(Sample const * /*bufferIn*/, const nFrame /*time*/) { }
    Sample const * output() { return _bufferOut.data(); }

protected:
    Buffer _bufferOut;
    nFrame _bufferSize;
    nFrame _time = 0;
    nFrame _left = 0;
    nFrame _right = 0;
};

#endif // ABSTRACTMODULE_H
