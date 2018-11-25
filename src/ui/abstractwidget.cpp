#include "abstractwidget.h"


void AbstractWidget::update_(void* status_)
{
    _status = status_;
    QWidget::update();
}

QSize AbstractWidget::minimumSizeHint() const
{
    QFontMetrics metrics(QApplication::font());
    return QSize(
        metrics.width(property("displayName").toString()) + 15,
        40
    );
}

void AbstractWidget::paintEvent(QPaintEvent *event)
{
    QRect rectFrame;
    QRect rectName;
    QRect rectContent;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // COMPUTE RECTS
    rectFrame = event->rect().adjusted(3, 3, -3, -3);
    rectName = event->rect().adjusted(3, 3, -3, -event->rect().height() + 25);
    rectContent = event->rect().adjusted(9, 30, -10, -10);

    // FRAME
    if( _selected ) {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(Qt::white, 3.0));
    } else {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(Qt::darkGray, 3.0));
    }
    painter.drawRoundedRect(rectFrame, 5.0, 5.0);

    // NAME
    if( _selected ) {
        painter.setBrush(Qt::white);
        painter.setPen(Qt::NoPen);
        painter.drawRect(rectName);
        painter.setPen(Qt::black);
    } else {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(Qt::white);
    }
    painter.drawText(rectName, Qt::AlignCenter, property("displayName").toString());

    // ACTUAL PAINT
    if( _status != nullptr )
        paint_(rectContent, _status);
}

void AbstractWidget::mousePressEvent(QMouseEvent *event) {
    if( event->button() == Qt::LeftButton ) {
        _selected = !_selected;
        emit selectedChanged(_selected);
        QWidget::update();
    }
}
