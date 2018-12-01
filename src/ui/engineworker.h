#ifndef ENGINEWORKER_H
#define ENGINEWORKER_H

#include <QObject>
#include "audio/engine.h"

class EngineWorker : public QObject
{
    Q_OBJECT
public:
    explicit EngineWorker(QObject *parent = nullptr) : QObject(parent) {}
    EngineStatus status() { return _engine.status(); }
    void update(EngineStatus status_) { _engine.update(status_); }
private:
    Engine _engine;

signals:
    void finished();
    void error(QString error);

public slots:
    void process() { _engine.start(); }
};

#endif // ENGINEWORKER_H
