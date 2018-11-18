#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <cstdint>
#include <vector>

#define SAMPLE_RATE 48000
#define LEVEL_METER_RMS_SAMPLE_COUNT 30
#define LEVEL_METER_AVERAGE_CYCLE_COUNT 30

typedef uint_fast32_t nFrame;
typedef float Sample; // PiSound supports only up to 32 bits
typedef std::vector<Sample> SampleVector;

#endif // TYPEDEFS_H
