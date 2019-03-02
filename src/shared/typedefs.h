#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <cstdint>
#include <vector>

typedef uint_fast32_t nFrame;
typedef uint_fast32_t nSequenceStep;
typedef uint_fast32_t nPpqn;  // Pulses per quarter note
typedef float Sample; // PiSound supports only up to 32 bits
typedef std::vector<Sample> Buffer;

#endif // TYPEDEFS_H

