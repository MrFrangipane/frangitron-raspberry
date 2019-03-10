#include "assert.h"
#include "ui/mainwindow.h"
#include "ui/engineworker.h"
#include "engine/engine.h"
#include <QObject>
#include <QApplication>
#include <QThread>


int main(int argc, char *argv[])
{
    // Type Size Assertions
    assert(sizeof(float) * 8 == 32);
    assert(sizeof(double) * 8 == 64);

    // CONFIG
    Configuration *configuration = new Configuration();
    configuration->modules[0].name = "IN";
    configuration->modules[0].type = "levelMeter";
    configuration->modules[0].wireIndex = -1;
    configuration->modules[0].overrides[10].active = true; // locked
    configuration->modules[0].overrides[10].value = 1.0;   // level
    configuration->modules[0].layout.row = 0;
    configuration->modules[0].layout.col = 0;
    configuration->modules[0].layout.rowSpan = 3;
    configuration->modules[0].layout.colSpan = 1;

    configuration->modules[1].name = "IN FILTER";
    configuration->modules[1].type = "filter";
    configuration->modules[1].wireIndex = 0;
    configuration->modules[1].layout.row = 0;
    configuration->modules[1].layout.col = 1;

    configuration->modules[2].name = "IN COMP";
    configuration->modules[2].type = "compressor";
    configuration->modules[2].wireIndex = 1;
    configuration->modules[2].layout.row = 1;
    configuration->modules[2].layout.col = 1;

    configuration->modules[3].name = "KICK 1";
    configuration->modules[3].type = "kickSynth";
    configuration->modules[3].wireIndex = 2;
    configuration->modules[3].midiNote = 36;
    configuration->modules[3].layout.row = 0;
    configuration->modules[3].layout.col = 2;

    configuration->modules[4].name = "KICK 2";
    configuration->modules[4].type = "kickSynth";
    configuration->modules[4].wireIndex = 3;
    configuration->modules[4].midiNote = 38;
    configuration->modules[4].layout.row = 0;
    configuration->modules[4].layout.col = 3;

    configuration->modules[5].name = "KICK 3";
    configuration->modules[5].type = "kickSynth";
    configuration->modules[5].wireIndex = 4;
    configuration->modules[5].midiNote = 40;
    configuration->modules[5].layout.row = 0;
    configuration->modules[5].layout.col = 4;

    configuration->modules[6].name = "KICK 4";
    configuration->modules[6].type = "kickSynth";
    configuration->modules[6].wireIndex = 5;
    configuration->modules[6].midiNote = 41;
    configuration->modules[6].layout.row = 0;
    configuration->modules[6].layout.col = 5;

    configuration->modules[7].name = "KICK 5";
    configuration->modules[7].type = "kickSynth";
    configuration->modules[7].wireIndex = 6;
    configuration->modules[7].midiNote = 43;
    configuration->modules[7].layout.row = 0;
    configuration->modules[7].layout.col = 6;

    configuration->modules[8].name = "KICK 6";
    configuration->modules[8].type = "kickSynth";
    configuration->modules[8].wireIndex = 7;
    configuration->modules[8].midiNote = 45;
    configuration->modules[8].layout.row = 0;
    configuration->modules[8].layout.col = 7;

    configuration->modules[9].type = "dummy";
    configuration->modules[9].layout.row = 0;
    configuration->modules[9].layout.col = 8;
    configuration->modules[9].layout.colStretch = 100;

    configuration->modules[10].name = "OUT";
    configuration->modules[10].type = "levelMeter";
    configuration->modules[10].wireIndex = 8;
    configuration->modules[10].layout.row = 0;
    configuration->modules[10].layout.col = 9;
    configuration->modules[10].layout.rowSpan = 3;
    configuration->modules[10].layout.colSpan = 1;

    // APPLICATION
    QThread::currentThread()->setObjectName("FrangUi");
    QApplication a(argc, argv);

    // ENGINE THREAD
    QThread* engineThread = new QThread();
    engineThread->setObjectName("FrangAudioMidi");
    EngineWorker* engineWorker = new EngineWorker(configuration);
    engineWorker->moveToThread(engineThread);

    QObject::connect(engineThread, SIGNAL(started()), engineWorker, SLOT(process()));
    QObject::connect(engineWorker, SIGNAL(finished()), engineThread, SLOT(quit()));
    QObject::connect(engineWorker, SIGNAL(finished()), engineWorker, SLOT(deleteLater()));
    QObject::connect(engineWorker, SIGNAL(finished()), engineThread, SLOT(deleteLater()));

    engineThread->start();

    // UI
    MainWindow w(configuration, engineWorker);
    w.show();

    return a.exec();
}
//*/

/*
 * This is to compute A Weighting filters
#include "iostream"
#include <cmath>
#include "sndfile.hh"
#include "audio/_aweighting.h".h"


int main(int argc, char *argv[])
{
    SndfileHandle f_whitenoise = SndfileHandle("/home/frangi/Downloads/whitenoise.wav");

    float max = 0.0;
    std::cout << "Format " << f_whitenoise.format() << std::endl;

    for( int cutoff = 0; cutoff <= 0; cutoff ++ ) {

        SndfileHandle f_output = SndfileHandle(
            "/home/frangi/Downloads/out_" + std::to_string(cutoff) + ".wav",
            SFM_WRITE,
            f_whitenoise.format(),
            f_whitenoise.channels(),
            f_whitenoise.samplerate()
        );

        _AWeighting a_weighting;

        std::cout << "/home/frangi/Downloads/out_" + std::to_string(cutoff) + ".wav : ";

        for( int frame = 0; frame < f_whitenoise.frames(); frame++ ) {
            short buffer[1];
            f_whitenoise.read(buffer, 1);

            float temp = (float)buffer[0] / 32767.0;

            temp = a_weighting.process(temp);

            max = std::fmax(max, temp);

            buffer[0] = (short)(temp * 32767);

            f_output.write(buffer, 1);
        }

        f_whitenoise.seek(0, 0);
    }

    std::cout << max;

    return 0;
}
//*/
