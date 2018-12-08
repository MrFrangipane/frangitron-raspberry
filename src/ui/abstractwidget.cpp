#include "abstractwidget.h"


void AbstractWidget::update_(const ModuleStatus status)
{
    if( status.params[0].name.empty() ) return;
    while( _isReadingStatus ) { }
    _isWritingStatus = true;
    _status = status;
    _isWritingStatus = false;
    QWidget::update();
}

QSize AbstractWidget::minimumSizeHint() const
{
    QFontMetrics metrics(QApplication::font());
    return QSize(
        std::max(metrics.width(property("displayName").toString()) + 15, _minimumWidth),
        std::max(40, _minimumHeight)
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
    while( _isWritingStatus ) { }
    _isReadingStatus = true;
    paint_(rectContent);
    _isReadingStatus = false;
}

void AbstractWidget::mousePressEvent(QMouseEvent *event) {
    if( event->button() == Qt::LeftButton ) {
        _selected = !_selected;
        emit selectedChanged(_selected);
        QWidget::update();
    }
}
