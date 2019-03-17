#ifndef DJDECK_H
#define DJDECK_H

#include "shared/typedefs.h"
#include "audio/abstractmodule.h"


class DjDeck : public AbstractModule
{
public:
    DjDeck(const nFrame bufferSize = 0) :
        AbstractModule(bufferSize) { }
};

#endif // DJDECK_H
