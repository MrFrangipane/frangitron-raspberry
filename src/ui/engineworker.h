#ifndef ENGINEWORKER_H
#define ENGINEWORKER_H

#include <QObject>
#include "engine/engine.h"

class EngineWorker : public QObject
{
    Q_OBJECT
public:
    explicit EngineWorker(QObject *parent = nullptr) : QObject(parent) {}
    void setStatusCallbacks(void * uiPtr, GetStatusCallback callbackGet, SetStatusCallback callbackSet) {
        _engine.setUiCallbacks(uiPtr, callbackGet, callbackSet);
    }
private:
    Engine _engine;

signals:
    void finished();
    void error(QString error);

public slots:
    void process() { _engine.start(); }
};

#endif // ENGINEWORKER_H
