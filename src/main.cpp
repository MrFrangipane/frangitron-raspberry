#include "assert.h"
#include "ui/mainwindow.h"
#include <QApplication>
#include <QStyle>
#include <QStyleFactory>


int main(int argc, char *argv[])
{
    // Type Size Assertions
    assert(sizeof(float) * 8 == 32);
    assert(sizeof(double) * 8 == 64);

    // Main
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    MainWindow w;
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
