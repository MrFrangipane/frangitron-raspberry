#ifndef AUDIOHELPERS_H
#define AUDIOHELPERS_H

#include <cmath>

inline float lerp(float start, float end, float factor) { return (1.0 - factor) * start + factor * end; }

inline float dB2Gain(float dB) { return std::pow(10.0, dB / 20.0); }

inline float gain2dB(float gain) { return 20.0 * std::log10(gain); }

#endif // AUDIOHELPERS_H

