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
    QRect rectFrame = event->rect();
    QRect rectName = QRect(rectFrame.left(), rectFrame.top(), rectFrame.width(), UI_TITLE_HEIGHT);
    QRect rectOutMeter;
    QRect rectContent;
    if( _drawMeter )
    {
        rectContent = event->rect().adjusted(
            UI_MARGIN,
            UI_MARGIN + UI_TITLE_HEIGHT,
            -UI_MARGIN - UI_MARGIN - UI_OUTMETER_WIDTH,
            -UI_MARGIN
        );
        rectOutMeter = QRect(
            rectContent.right() + UI_MARGIN,
            rectContent.top() + (rectContent.height() - UI_OUTMETER_HEIGHT) / 2,
            UI_OUTMETER_WIDTH,
            UI_OUTMETER_HEIGHT
        );
    }
    else {
        rectContent = event->rect().adjusted(
            UI_MARGIN,
            UI_MARGIN + UI_TITLE_HEIGHT,
            -UI_MARGIN,
            -UI_MARGIN
        );
    }

    // FRAME
    if( _selected ) {
        drawRect(painter, rectFrame, Qt::white, UI_LINE_WIDTH);
    } else {
        drawRect(painter, rectFrame, _darkGrey, UI_LINE_WIDTH);
    }

    // NAME
    if( _selected ) {
        fillRect(painter, rectName, Qt::white);
        painter.setPen(Qt::black);
    } else {
        fillRect(painter, rectName, _darkGrey);
        painter.setPen(Qt::white);
    }
    painter.drawText(rectName, Qt::AlignCenter, property("displayName").toString());

    // METER OUT
    if( _drawMeter ) {
        fillRect(painter, rectOutMeter, _darkGrey);
        if( _status.is_clipping ) {
            fillRect(painter, rectOutMeter.adjusted(0, std::fmin(rectOutMeter.height(), -_status.levelOut * rectOutMeter.height() * UI_LEVEL_MAGIC_COEFF), 0, 0), Qt::red);
        } else {
            fillRect(painter, rectOutMeter.adjusted(0, std::fmin(rectOutMeter.height(), -_status.levelOut * rectOutMeter.height() * UI_LEVEL_MAGIC_COEFF), 0, 0), Qt::white);
        }
    }

    // ACTUAL PAINT
    paint_(rectContent);
}

void AbstractWidget::mousePressEvent(QMouseEvent *event) {
    if( event->button() == Qt::LeftButton && !_selected) {
        _selected = true;
        emit selectedChanged(_selected);
        QWidget::update();
    }
}
