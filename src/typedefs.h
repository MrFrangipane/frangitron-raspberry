#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <cmath>
#include <vector>
#include <string>
#include <cstdint>

#define SAMPLE_RATE 48000
#define SECOND_PER_SAMPLE 0.000020833
#define LEVEL_METER_RMS_SAMPLE_COUNT 60
#define LEVEL_METER_AVERAGE_CYCLE_COUNT 15
#define COMPRESSOR_UPDATE_SAMPLE_COUNT 60

#define UI_FRAMERATE 30
#define UI_OUTMETER_HEIGHT 60

typedef uint_fast32_t nFrame;
typedef float Sample; // PiSound supports only up to 32 bits
typedef std::vector<Sample> Buffer;

inline float lerp(float start, float end, float factor) { return (1.0 - factor) * start + factor * end; }

inline float dB2Gain(float dB) { return std::pow(10.0, dB / 20.0); }

inline float gain2dB(float gain) { return 20.0 * std::log10(gain); }

struct ModuleParameter {
    std::string name = "";
    std::string format = "%1";
    float value = 0.0;
    float min = 0.0;
    float max = 1.0;
    float step = 0.0078; // 1.0 / 128
    bool visible = false;
};


struct ModuleStatus {
    ModuleParameter params[16];
    ModuleParameter levelOut;
};


struct EngineStatus {
    ModuleStatus modules[16];
};

#endif // TYPEDEFS_H
