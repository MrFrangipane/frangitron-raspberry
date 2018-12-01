#ifndef AUDIOWORKER_H
#define AUDIOWORKER_H

#include <QObject>
#include "audio/engine.h"

class AudioWorker : public QObject
{
    Q_OBJECT
public:
    explicit AudioWorker(QObject *parent = nullptr) : QObject(parent) {}
    EngineStatus status() { return _engine.status(); }
    void update(EngineStatus status_) { _engine.update(status_); }
private:
    Engine _engine;

signals:
    void finished();
    void error(QString error);

public slots:
    void process();
};

#endif // AUDIOWORKER_H
