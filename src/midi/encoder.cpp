#include "encoder.h"

bool Encoder::clicked(nFrame time)
{
    return false;
}

bool Encoder::pressed(nFrame time)
{
    return _pressed;
}

float Encoder::increment(nFrame time)
{
    return _increment;
}
