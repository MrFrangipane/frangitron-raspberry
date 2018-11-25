#include "abstractwidget.h"

#include "levelmeter.h"
#include <iostream>

void AbstractWidget::update_(void* status_)
{
    _status = status_;
    std::cout << ((LevelMeterStatus*)_status)->rmsL << " ";
    QWidget::update();
}

void AbstractWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // COMPUTE RECTS
    QRect frame(event->rect().adjusted(3, 3, -3, -3));
    QRect inside(event->rect().adjusted(9, 9, -9, -9));

    // FRAME
    if( _selected ) {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(Qt::white, 3.0));
    } else {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(Qt::darkGray, 3.0));
    }
    painter.drawRoundedRect(frame, 5.0, 5.0);

    // ACTUAL PAINT
    paint_(inside, _status);
}


void AbstractWidget::mousePressEvent(QMouseEvent *event) {
    if( event->button() == Qt::LeftButton ) {
        _selected = !_selected;
        emit selectedChanged(_selected);
        QWidget::update();
    }
}
