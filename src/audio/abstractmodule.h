#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

#include "typedefs.h"


template<typename T>
class AbstractModule
{
public:
    AbstractModule(const nFrame bufferSize = 0) :
        _bufferSize(bufferSize)
    {
        _bufferOut.reserve(bufferSize * 2);
        update(T());
    }
    virtual T status() { return T(); }
    virtual void update(T /*status_*/) { }
    Sample const * output() { return _bufferOut.data(); }
    virtual void process(Sample const * /*bufferIn*/, const nFrame /*time*/) { }

protected:
    Buffer _bufferOut;
    nFrame _bufferSize;
    nFrame _time = 0;
    nFrame _left = 0;
    nFrame _right = 0;
};

#endif // ABSTRACTMODULE_H
