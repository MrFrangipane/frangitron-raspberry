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
    initConfig(configuration);

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
