#ifndef ENGINEWORKER_H
#define ENGINEWORKER_H

#include <QObject>
#include "audio/engine.h"

class EngineWorker : public QObject
{
    Q_OBJECT
public:
    explicit EngineWorker(QObject *parent = nullptr) : QObject(parent) {}
    EngineStatus const status() { return _engine.status(); }
    void setStatusCallback(void * uiPtr, EngineStatusCallback callback) { _engine.setStatusCallback(uiPtr, callback); }
private:
    Engine _engine;

signals:
    void finished();
    void error(QString error);

public slots:
    void process() { _engine.start(); }
};

#endif // ENGINEWORKER_H
