#include "audioworker.h"

AudioWorker::AudioWorker(QObject *parent) : QObject(parent) {}


void AudioWorker::process()
{
    _audiomidi.start();
}
