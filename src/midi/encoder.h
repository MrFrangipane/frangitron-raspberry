#ifndef ENCODER_H
#define ENCODER_H

#include "shared/constants.h"
#include "shared/typedefs.h"
#include "shared/masterclock.h"


class Encoder
{
public:
    Encoder() {}
    void setIncrement(float increment, ClockStatus time) { _increment = increment; _lastIncrementUpdate = time.engineFrame; }
    void setPressed(bool pressed, ClockStatus time) { _pressed = pressed; _lastPressedUpdate = time.engineFrame; }
    bool clicked(ClockStatus /*time*/);
    bool pressed(ClockStatus /*time*/);
    float increment(ClockStatus /*time*/);


private:
    float _increment = 0.0;
    bool _pressed = 0.0;
    nFrame _lastIncrementUpdate = 0;
    nFrame _lastPressedUpdate = 0;
};

#endif // ENCODER_H
