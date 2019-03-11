#include "shared/configuration.h"


void initConfig(Configuration* conf)
{
    initModules(conf);
    initSamples(conf);
}

// Here we put ugly stuff that makes engine and Ui code more generic
// expecting to parse a JSON file at some point
void initSamples(Configuration* conf)
{
    conf->audioClips[0].name = "Kick 01";
    conf->audioClips[0].filepath = "/var/frangitron/samples/KIT111AI.wav";
    conf->audioClips[0].channelCount = 2;
    conf->audioClips[0].frameCount = 13264;

    conf->audioClips[1].name = "Kick 02";
    conf->audioClips[1].filepath = "/var/frangitron/samples/BD11_WAV.wav";
    conf->audioClips[1].channelCount = 2;
    conf->audioClips[1].frameCount = 29911;

    conf->audioClips[2].name = "Snare 01";
    conf->audioClips[2].filepath = "/var/frangitron/samples/SN_11.wav";
    conf->audioClips[2].channelCount = 2;
    conf->audioClips[2].frameCount = 20672;

    conf->audioClips[3].name = "Snare 02";
    conf->audioClips[3].filepath = "/var/frangitron/samples/SN_12.wav";
    conf->audioClips[3].channelCount = 2;
    conf->audioClips[3].frameCount = 16024;
}


// Here we put ugly stuff that makes engine and Ui code more generic
// expecting to parse a JSON file at some point
void initModules(Configuration* conf)
{
    conf->modules[0].name = "IN";
    conf->modules[0].type = "levelMeter";
    conf->modules[0].wireIndex = -1;
    conf->modules[0].overrides[10].active = true; // locked
    conf->modules[0].overrides[10].value = 1.0;   // level
    conf->modules[0].layout.row = 0;
    conf->modules[0].layout.col = 0;
    conf->modules[0].layout.rowSpan = 3;
    conf->modules[0].layout.colSpan = 1;

    conf->modules[1].name = "IN FILTER";
    conf->modules[1].type = "filter";
    conf->modules[1].wireIndex = 0;
    conf->modules[1].layout.row = 0;
    conf->modules[1].layout.col = 1;

    conf->modules[2].name = "IN COMP";
    conf->modules[2].type = "compressor";
    conf->modules[2].wireIndex = 1;
    conf->modules[2].layout.row = 1;
    conf->modules[2].layout.col = 1;

    conf->modules[3].name = "KICK 1";
    conf->modules[3].type = "kickSynth";
    conf->modules[3].wireIndex = 2;
    conf->modules[3].midiNote = 36;
    conf->modules[3].layout.row = 0;
    conf->modules[3].layout.col = 2;

    conf->modules[4].name = "SAMPLE 1";
    conf->modules[4].type = "samplePlayer";
    conf->modules[4].wireIndex = 3;
    conf->modules[4].midiNote = 38;
    conf->modules[4].layout.row = 0;
    conf->modules[4].layout.col = 3;

    /*conf->modules[5].name = "KICK 3";
    conf->modules[5].type = "kickSynth";
    conf->modules[5].wireIndex = 4;
    conf->modules[5].midiNote = 40;
    conf->modules[5].layout.row = 0;
    conf->modules[5].layout.col = 4;

    conf->modules[6].name = "KICK 4";
    conf->modules[6].type = "kickSynth";
    conf->modules[6].wireIndex = 5;
    conf->modules[6].midiNote = 41;
    conf->modules[6].layout.row = 0;
    conf->modules[6].layout.col = 5;

    conf->modules[7].name = "KICK 5";
    conf->modules[7].type = "kickSynth";
    conf->modules[7].wireIndex = 6;
    conf->modules[7].midiNote = 43;
    conf->modules[7].layout.row = 0;
    conf->modules[7].layout.col = 6;

    conf->modules[8].name = "KICK 6";
    conf->modules[8].type = "kickSynth";
    conf->modules[8].wireIndex = 7;
    conf->modules[8].midiNote = 45;
    conf->modules[8].layout.row = 0;
    conf->modules[8].layout.col = 7;*/

    conf->modules[5].type = "dummy";
    conf->modules[5].layout.row = 0;
    conf->modules[5].layout.col = 8;
    conf->modules[5].layout.colStretch = 100;

    conf->modules[6].name = "OUT";
    conf->modules[6].type = "levelMeter";
    conf->modules[6].wireIndex = 4;
    conf->modules[6].layout.row = 0;
    conf->modules[6].layout.col = 9;
    conf->modules[6].layout.rowSpan = 3;
    conf->modules[6].layout.colSpan = 1;
}
