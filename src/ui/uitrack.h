#ifndef UITRACK_H
#define UITRACK_H

#include <QString>
#include <QRect>
#include <QWidget>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include "typedefs.h"
#include "track.h"

class UiTrack : public QWidget
{
    Q_OBJECT
public:
    explicit UiTrack(QWidget *parent = nullptr) {}
    void update_(TrackStatus status_);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    TrackStatus _status;
};

#endif // UITRACK_H
