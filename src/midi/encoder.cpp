#include "encoder.h"

bool Encoder::clicked(ClockStatus /*time*/)
{
    return false;
}

bool Encoder::pressed(ClockStatus /*time*/)
{
    return _pressed;
}

float Encoder::increment(ClockStatus /*time*/)
{
    return _increment;
}
