#ifndef AUDIOWORKER_H
#define AUDIOWORKER_H

#include <QObject>
#include "audiomidi.h"

class AudioWorker : public QObject
{
    Q_OBJECT
public:
    explicit AudioWorker(QObject *parent = nullptr);
private:
    AudioMidi _audiomidi;

signals:
    void finished();
    void error(QString error);

public slots:
    void process();

};

#endif // AUDIOWORKER_H
