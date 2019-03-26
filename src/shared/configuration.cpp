#include "shared/configuration.h"


void initConfig(Configuration* conf)
{
    initPatch(conf);
    initSamples(conf);
    initDjTracks(conf);
}

// Here we put ugly stuff that makes engine and Ui code more generic
// expecting to parse a JSON file at some point (instead of doing that)
void initSamples(Configuration* conf)
{
    conf->samples[0].name = "Kick 01";    
    conf->samples[0].filepath = "/var/frangitron/samples/KIT111AI.wav";

    conf->samples[1].name = "Kick 02";
    conf->samples[1].filepath = "/var/frangitron/samples/BD11_WAV.wav";

    conf->samples[2].name = "Snare 01";
    conf->samples[2].filepath = "/var/frangitron/samples/SN_11.wav";

    conf->samples[3].name = "Snare 02";
    conf->samples[3].filepath = "/var/frangitron/samples/SN_12.wav";
}


void initDjTracks(Configuration* conf)
{
    conf->djTracks[0].name = "Aviate Intro";
    conf->djTracks[0].filepath = "/var/frangitron/djtracks/01-intro.wav";

    conf->djTracks[1].name = "Aviate Bridge";
    conf->djTracks[1].filepath = "/var/frangitron/djtracks/02-bridge.wav";

    conf->djTracks[2].name = "Aviate Loop";
    conf->djTracks[2].filepath = "/var/frangitron/djtracks/03-loop.wav";

    conf->djTracks[3].name = "Aviate Bridge";
    conf->djTracks[3].filepath = "/var/frangitron/djtracks/04-bridge.wav";

    conf->djTracks[4].name = "Temp Break";
    conf->djTracks[4].filepath = "/var/frangitron/djtracks/01-break.wav";

    conf->djTracks[5].name = "Temp Long";
    conf->djTracks[5].filepath = "/var/frangitron/djtracks/02-long.wav";
}


void initPatch(Configuration* conf)
{
    // INPUT
    conf->modules[0].name = "IN";
    conf->modules[0].type = "levelMeter";
    conf->modules[0].inputIndex = MODULE_INPUT_SOUNDCARD;
    conf->modules[0].overrides[10].isActive = true; // locked
    conf->modules[0].overrides[10].value = 1.0;     // level
    conf->modules[0].layout.row = 0;
    conf->modules[0].layout.col = 2;
    conf->modules[0].routedToMasterBus = true;

    // DECK A
    conf->modules[1].name = "DECK A";
    conf->modules[1].type = "djDeck";
    conf->modules[1].inputIndex = MODULE_INPUT_NONE;
    conf->modules[1].layout.row = 1;
    conf->modules[1].layout.col = 0;
    conf->modules[1].layout.colSpan = 2;
    conf->modules[1].overrides[2].isActive = true;
    conf->modules[1].overrides[2].value = -1;  // No Audio File

    conf->modules[2].name = "FILTER A";
    conf->modules[2].type = "filter";
    conf->modules[2].inputIndex = 1;
    conf->modules[2].layout.row = 0;
    conf->modules[2].layout.col = 0;

    conf->modules[3].name = "A";
    conf->modules[3].type = "levelMeter";
    conf->modules[3].inputIndex = 2;
    conf->modules[3].layout.row = 1;
    conf->modules[3].layout.col = 2;
    conf->modules[3].overrides[2].isActive = true;
    conf->modules[3].overrides[2].value = -12.0;  // dB
    conf->modules[3].routedToMasterBus = true;

    // DECK B
    conf->modules[4].name = "DECK B";
    conf->modules[4].type = "djDeck";
    conf->modules[4].inputIndex = MODULE_INPUT_NONE;
    conf->modules[4].layout.row = 1;
    conf->modules[4].layout.col = 4;
    conf->modules[4].layout.colSpan = 2;
    conf->modules[4].overrides[2].isActive = true;
    conf->modules[4].overrides[2].value = 5;  // Audio File 5

    conf->modules[5].name = "FILTER B";
    conf->modules[5].type = "filter";
    conf->modules[5].inputIndex = 4;
    conf->modules[5].layout.row = 0;
    conf->modules[5].layout.col = 5;

    conf->modules[6].name = "B";
    conf->modules[6].type = "levelMeter";
    conf->modules[6].inputIndex = 5;
    conf->modules[6].layout.row = 1;
    conf->modules[6].layout.col = 3;
    conf->modules[6].overrides[2].isActive = true;
    conf->modules[6].overrides[2].value = -12.0;  // dB
    conf->modules[6].routedToMasterBus = true;

    // MASTER OUT
    conf->modules[7].name = "OUT";
    conf->modules[7].type = "levelMeter";
    conf->modules[7].inputIndex = MODULE_INPUT_MASTER_BUS;
    conf->modules[7].layout.row = 0;
    conf->modules[7].layout.col = 3;
    conf->modules[7].overrides[2].isActive = true;
    conf->modules[7].overrides[2].value = -6.0;  // dB

    // DUMMIES MUST BE AT THE END
    conf->modules[8].type = "dummy";
    conf->modules[8].layout.row = 0;
    conf->modules[8].layout.col = 1;
    conf->modules[8].layout.colStretch = 100;

    conf->modules[9].type = "dummy";
    conf->modules[9].layout.row = 0;
    conf->modules[9].layout.col = 4;
    conf->modules[9].layout.colStretch = 100;
}
