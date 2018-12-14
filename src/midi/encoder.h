#ifndef ENCODER_H
#define ENCODER_H

#include "shared/constants.h"
#include "shared/typedefs.h"

class Encoder
{
public:
    Encoder() {}
    void setIncrement(float increment, nFrame time) { _increment = increment; _lastIncrementUpdate = time; }
    void setPressed(bool pressed, nFrame time) { _pressed = pressed; _lastPressedUpdate = time; }
    bool clicked(nFrame time);
    bool pressed(nFrame time);
    float increment(nFrame time);


private:
    float _increment = 0.0;
    bool _pressed = 0.0;
    nFrame _lastIncrementUpdate = 0;
    nFrame _lastPressedUpdate = 0;
};

#endif // ENCODER_H
