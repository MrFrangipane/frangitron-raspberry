#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <cstdint>
#include <vector>

typedef uint_fast32_t nFrame;
typedef float Sample; // PiSound supports only up to 32 bits
typedef std::vector<Sample> Buffer;

#endif // TYPEDEFS_H

