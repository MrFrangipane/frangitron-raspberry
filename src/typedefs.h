#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <cstdint>
#include <vector>

#define SAMPLE_RATE 48000
#define SECOND_PER_SAMPLE 0.000020833
#define LEVEL_METER_RMS_SAMPLE_COUNT 60
#define LEVEL_METER_AVERAGE_CYCLE_COUNT 15
#define COMPRESSOR_UPDATE_SAMPLE_COUNT 60

#define UI_FRAMERATE 30
#define UI_VU_MIN -220
#define UI_VU_MARGIN 10

typedef uint_fast32_t nFrame;
typedef float Sample; // PiSound supports only up to 32 bits
typedef std::vector<Sample> Buffer;

inline float lerp(float start, float end, float factor) { return (1.0 - factor) * start + factor * end; }

#endif // TYPEDEFS_H
