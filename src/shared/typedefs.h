#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <cstdint>
#include <vector>

typedef uint_fast32_t nCue;
typedef uint_fast32_t nFrame;
typedef uint_fast32_t nSample;
typedef uint_fast32_t nBar;
typedef uint_fast32_t nBeat;
typedef uint_fast32_t nStep; // Sequencer Step (16 per bar)
typedef uint_fast32_t nPulse;  // 24 pulses per quarter note on Elektron MD
typedef double nSecond;
typedef float Sample; // PiSound supports only up to 32 bits
typedef std::vector<Sample> Buffer;

#endif // TYPEDEFS_H

