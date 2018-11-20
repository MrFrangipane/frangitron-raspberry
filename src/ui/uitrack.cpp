#include "uitrack.h"

void UiTrack::update_(TrackStatus status_)
{
    _status = status_;
    QWidget::update();
}

void UiTrack::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);

    QRect vu(event->rect().adjusted(
        UI_VU_MARGIN,
        0,
        -(float)event->rect().width() / 3,
        -1
    ));
    QRect comp(event->rect().adjusted(
        2.0 * (float)event->rect().width() / 3,
        0,
        -UI_VU_MARGIN,
        -1
    ));

    // VU
    painter.setBrush(Qt::white);

    painter.drawRect(vu.adjusted(
        0,
        (_status.levelInL / UI_VU_MIN) * vu.height(),
        -(vu.width() / 2),
        0
    ));

    painter.drawRect(vu.adjusted(
        vu.width() / 2,
        (_status.levelInR / UI_VU_MIN) * vu.height(),
        0,
        0
    ));

    // COMP
    if( _status.compressor.gate ) painter.setPen(QPen(Qt::red, 2.0));
    else painter.setPen(QPen(Qt::yellow, 2.0));
    painter.drawLine(
        UI_VU_MARGIN,
        (_status.compressor.threshold / UI_VU_MIN) * event->rect().height(),
        vu.width() + UI_VU_MARGIN,
        (_status.compressor.threshold / UI_VU_MIN) * event->rect().height()
    );

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::red);

    painter.drawRect(comp.adjusted(
        0,
        0,
        0,
        -_status.compressor.level * comp.height()
    ));

    // FRAME
    /*
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::white);
    painter.drawRect(event->rect());
    painter.setPen(Qt::yellow);
    painter.drawRect(vu);
    painter.setPen(Qt::green);
    painter.drawRect(comp);
    */
}
