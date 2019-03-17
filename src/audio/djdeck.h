#ifndef DJDECK_H
#define DJDECK_H

#include "shared/typedefs.h"
#include "audio/abstractmodule.h"


class DjDeck : public AbstractModule
{
public:
    DjDeck(const nFrame bufferSize = 0, int deckIndex = -1) :
        AbstractModule(bufferSize),
        _deckIndex(deckIndex)
    { }

private:
    int _deckIndex;
};

#endif // DJDECK_H
