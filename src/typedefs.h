#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <cstdint>
#include <vector>

#define SAMPLE_RATE 48000
#define LEVEL_METER_RMS_SAMPLE_COUNT 30
#define LEVEL_METER_AVERAGE_CYCLE_COUNT 30
#define COMPRESSOR_UPDATE_SAMPLE_COUNT 30
#define COMPRESSOR_LERP_SAMPLE_COUNT 30

typedef uint_fast32_t nFrame;
typedef float Sample; // PiSound supports only up to 32 bits
typedef std::vector<Sample> SampleVector;

inline float lerp(float start, float end, float factor) { return (1.0 - factor) * start + factor * end; }

#endif // TYPEDEFS_H
