#ifndef ENGINEWORKER_H
#define ENGINEWORKER_H

#include <QObject>
#include "engine/engine.h"
#include "shared/structures.h"
#include "shared/configuration.h"

class EngineWorker : public QObject
{
    Q_OBJECT
public:
    explicit EngineWorker(const Configuration * configuration, QObject *parent = nullptr) :
        QObject(parent),
        _engine(configuration)
        { }
    void setStatusCallbacks(void * uiPtr, GetStatusCallback callbackGet, SetStatusCallback callbackSet) {
        _engine.setUiCallbacks(uiPtr, callbackGet, callbackSet);
    }
    void stop() { _engine.stop(); }
private:
    Engine _engine;

signals:
    void finished();
    void error(QString error);

public slots:
    void process() { _engine.start(); }
};

#endif // ENGINEWORKER_H
