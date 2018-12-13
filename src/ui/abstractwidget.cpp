#include "abstractwidget.h"


void AbstractWidget::update_(const ModuleStatus status)
{
    _status = status;
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
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // COMPUTE RECTS
    QRect rectFrame = event->rect().adjusted(3, 3, -3, -3);
    QRect rectName = event->rect().adjusted(3, 3, -3, -event->rect().height() + 25);
    QRect rectOutMeter;
    QRect rectContent;
    if( _drawMeter )
    {
        rectContent = event->rect().adjusted(9, 30, -22, -10);

        rectOutMeter = event->rect().adjusted(event->rect().width() - 17, 30, -9, -10);
        rectOutMeter.setHeight(std::min(rectOutMeter.height(), UI_OUTMETER_HEIGHT));
        rectOutMeter.moveTop(rectContent.top() + (rectContent.height() - rectOutMeter.height()) / 2);

    }
    else {
        rectContent = event->rect().adjusted(9, 30, -10, -10);
    }

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
        painter.setBrush(Qt::darkGray);
        painter.setPen(Qt::NoPen);
        painter.drawRect(rectName);
        painter.setPen(Qt::white);
    }
    painter.drawText(rectName, Qt::AlignCenter, property("displayName").toString());

    // METER OUT
    if( _drawMeter ) {
        fillRect(painter, rectOutMeter, Qt::darkGray);
        fillRect(painter, rectOutMeter.adjusted(0, -_status.levelOut * rectOutMeter.height() * UI_LEVEL_MAGIC_COEFF, 0, 0), Qt::white);
    }

    // ACTUAL PAINT
    paint_(rectContent);
}

void AbstractWidget::mousePressEvent(QMouseEvent *event) {
    if( event->button() == Qt::LeftButton ) {
        _selected = !_selected;
        emit selectedChanged(_selected);
        QWidget::update();
    }
}
