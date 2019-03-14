#include "shared/configuration.h"


void initConfig(Configuration* conf)
{
    initModules(conf);
    initSamples(conf);
}

// Here we put ugly stuff that makes engine and Ui code more generic
// expecting to parse a JSON file at some point (instead of doing that)
void initSamples(Configuration* conf)
{
    conf->samples[0].name = "Kick 01";
    conf->samples[0].filepath = "/var/frangitron/samples/KIT111AI.wav";
    conf->samples[0].channelCount = 2;
    conf->samples[0].frameCount = 13264;

    conf->samples[1].name = "Kick 02";
    conf->samples[1].filepath = "/var/frangitron/samples/BD11_WAV.wav";
    conf->samples[1].channelCount = 2;
    conf->samples[1].frameCount = 29911;

    conf->samples[2].name = "Snare 01";
    conf->samples[2].filepath = "/var/frangitron/samples/SN_11.wav";
    conf->samples[2].channelCount = 2;
    conf->samples[2].frameCount = 20672;

    conf->samples[3].name = "Snare 02";
    conf->samples[3].filepath = "/var/frangitron/samples/SN_12.wav";
    conf->samples[3].channelCount = 2;
    conf->samples[3].frameCount = 16024;
}


void initClips(Configuration* conf)
{
    conf->clips[0].name = "Kick 01";
    conf->clips[0].filepath = "/var/frangitron/samples/KIT111AI.wav";
    conf->clips[0].channelCount = 2;
    conf->clips[0].frameCount = 13264;

    conf->clips[1].name = "Kick 01";
    conf->clips[1].filepath = "/var/frangitron/samples/KIT111AI.wav";
    conf->clips[1].channelCount = 2;
    conf->clips[1].frameCount = 13264;

    conf->clips[2].name = "Kick 01";
    conf->clips[2].filepath = "/var/frangitron/samples/KIT111AI.wav";
    conf->clips[2].channelCount = 2;
    conf->clips[2].frameCount = 13264;

    conf->clips[3].name = "Kick 01";
    conf->clips[3].filepath = "/var/frangitron/samples/KIT111AI.wav";
    conf->clips[3].channelCount = 2;
    conf->clips[3].frameCount = 13264;
}


void initModules(Configuration* conf)
{
    conf->modules[0].name = "IN";
    conf->modules[0].type = "levelMeter";
    conf->modules[0].inputIndex = -1;
    conf->modules[0].overrides[10].isActive = true; // locked
    conf->modules[0].overrides[10].value = 1.0;   // level
    conf->modules[0].layout.row = 0;
    conf->modules[0].layout.col = 0;
    conf->modules[0].layout.rowSpan = 3;
    conf->modules[0].layout.colSpan = 1;

    conf->modules[1].name = "IN FILTER";
    conf->modules[1].type = "filter";
    conf->modules[1].inputIndex = 0;
    conf->modules[1].layout.row = 0;
    conf->modules[1].layout.col = 1;

    conf->modules[2].name = "IN COMP";
    conf->modules[2].type = "compressor";
    conf->modules[2].inputIndex = 1;
    conf->modules[2].layout.row = 1;
    conf->modules[2].layout.col = 1;

    conf->modules[3].name = "KICK 1";
    conf->modules[3].type = "kickSynth";
    conf->modules[3].inputIndex = 2;
    conf->modules[3].midiNote = 36;
    conf->modules[3].layout.row = 0;
    conf->modules[3].layout.col = 2;

    conf->modules[4].name = "SAMPLE 1";
    conf->modules[4].type = "samplePlayer";
    conf->modules[4].inputIndex = 3;
    conf->modules[4].midiNote = 38;
    conf->modules[4].layout.row = 0;
    conf->modules[4].layout.col = 3;

    conf->modules[5].name = "SAMPLE 2";
    conf->modules[5].type = "samplePlayer";
    conf->modules[5].inputIndex = 4;
    conf->modules[5].midiNote = 40;
    conf->modules[5].layout.row = 0;
    conf->modules[5].layout.col = 4;

    conf->modules[6].type = "dummy";
    conf->modules[6].layout.row = 0;
    conf->modules[6].layout.col = 8;
    conf->modules[6].layout.colStretch = 100;

    conf->modules[7].name = "OUT";
    conf->modules[7].type = "levelMeter";
    conf->modules[7].inputIndex = 5;
    conf->modules[7].layout.row = 0;
    conf->modules[7].layout.col = 9;
    conf->modules[7].layout.rowSpan = 3;
    conf->modules[7].layout.colSpan = 1;
}
