#include "assert.h"
#include "ui/mainwindow.h"
#include <QApplication>
#include <QStyle>
#include <QStyleFactory>

/*
#include "sndfile.hh"
#include "audio/_samplefilter.h"
*/

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

    /*
    SndfileHandle f_whitenoise = SndfileHandle("/home/frangi/Downloads/whitenoise.wav");

    for( int cutoff = 0; cutoff <= 0; cutoff ++ ) {

        SndfileHandle f_output = SndfileHandle(
            "/home/frangi/Downloads/out_" + std::to_string(cutoff) + ".wav",
            SFM_WRITE,
            f_whitenoise.format(),
            f_whitenoise.channels(),
            f_whitenoise.samplerate()
        );

        _SampleFilter filter_hi;
        filter_hi.setCutoff(0.95);
        filter_hi.setResonance(0);

        _SampleFilter filter_low(HIPASS);
        filter_low.setCutoff(0.5);
        filter_low.setResonance(0);

        std::cout << filter_low.cutoff() << " " << filter_hi.cutoff() << std::endl;

        for( int frame = 0; frame < f_whitenoise.frames(); frame++ ) {
            short buffer[1];

            f_whitenoise.read(buffer, 1);

            buffer[0] = filter_low.process(buffer[0]) * 5.55;
            buffer[0] = filter_hi.process(buffer[0]) * 5.55;

            f_output.write(buffer, 1);

        }

        f_whitenoise.seek(0, 0);
    }

    return 0;
    */
}
