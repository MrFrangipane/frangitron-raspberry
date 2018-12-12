#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <cmath>
#include <vector>
#include <string>
#include <cstdint>

#define SAMPLE_RATE 48000
#define MAGIC_LEVEL_COEFF 100.0
#define SECOND_PER_SAMPLE 0.000020833
#define LEVEL_METER_RMS_SAMPLE_COUNT 60
#define LEVEL_METER_AVERAGE_CYCLE_COUNT 15
#define COMPRESSOR_UPDATE_SAMPLE_COUNT 60

#define UI_FRAMERATE 30
#define UI_OUTMETER_HEIGHT 60
#define UI_SHAFT_WIDTH 8
#define UI_LEVEL_MAGIC_COEFF 0.0045

typedef uint_fast32_t nFrame;
typedef float Sample; // PiSound supports only up to 32 bits
typedef std::vector<Sample> Buffer;

inline float lerp(float start, float end, float factor) { return (1.0 - factor) * start + factor * end; }

inline float dB2Gain(float dB) { return std::pow(10.0, dB / 20.0); }

inline float gain2dB(float gain) { return 20.0 * std::log10(gain); }

struct EncoderStatus {
    bool isPressed = false;
    int increment = 0;
};

struct ModuleParameter {
    std::string name = "";
    float value = 0.0;
    float min = 0.0;
    float max = 1.0;
    float step = 0.0078; // 1.0 / 128
    bool visible = false;
};

struct ModuleStatus {
    ModuleParameter params[16];
    float levelOut;
};

struct EngineStatus {
    EncoderStatus encoders[5];
    ModuleStatus modules[16];
    int selectedModule = -1;
};

#endif // TYPEDEFS_H
