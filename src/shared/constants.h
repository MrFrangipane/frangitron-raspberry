#ifndef CONSTANTS_H
#define CONSTANTS_H

#define BUFFER_SIZE 512
#define SAMPLE_RATE 48000
#define CHANNEL_COUNT 2
#define RECORDER_CACHE_BUFFER_COUNT 512 // 512 * 512 = about 5.5s
#define RECORDER_OUTPUT_FILE "/var/frangitron/recordings/frangitron_%d-%m-%Y_%H.%M.%S.wav"
#define RECORDER_SLEEP_MS 5 // Sleep 5ms if file write is ahead of engine
#define LEVEL_METER_RMS_SAMPLE_COUNT 60
#define LEVEL_METER_AVERAGE_CYCLE_COUNT 15
#define COMPRESSOR_UPDATE_SAMPLE_COUNT 60
#define CLIPPING_THRESOLD -8.2 // instant RMS dB
#define CLIPPING_PERSIST_BUFFER_COUNT 100 // about 0.2s

#define MIDI_NOTE_COUNT 128
#define MIDI_ENCODER_COUNT 5
#define MIDI_PUSHED_FACTOR 6
#define MIDI_CLICKED_TIME_TRESHOLD 128 // in audio frames

#define AUDIO_CLIP_MAX_COUNT 64
#define AUDIO_SAMPLE_MAX_COUNT 64
#define AUDIO_MARKER_COUNT_MAX 64

#define DECK_LENGTH_SECONDS 600 // 10 minutes

#define MODULE_MAX_COUNT 16
#define MODULE_PARAM_COUNT 16

#define UI_FRAMERATE 30
#define UI_MARGIN 8
#define UI_ARC_WIDTH 4
#define UI_LINE_WIDTH 2
#define UI_SHAFT_WIDTH 4
#define UI_TITLE_HEIGHT 25
#define UI_OUTMETER_WIDTH 6
#define UI_OUTMETER_HEIGHT 40
#define UI_MIN_CONTENT_SIZE 40
#define UI_LEVEL_HANDLE_SIZE 10
#define UI_LEVEL_MAGIC_COEFF 0.0045
#define UI_ASPECT_RATIO 0.95 // 800x480 as 16:9

#endif // CONSTANTS_H
