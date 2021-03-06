#include "abstractwidget.h"


void AbstractWidget::update_(const ModuleStatus status)
{
    _status = status;
    update();
}

QSize AbstractWidget::minimumSizeHint() const
{
    QFontMetrics metrics(QApplication::font());
    int titleWidth = metrics.width(property("displayName").toString());
    int contentWidth;

    if( _drawMeter ) {
        contentWidth = UI_MARGIN + _contentMinimumWidth + UI_MARGIN + UI_OUTMETER_WIDTH + UI_MARGIN;
    } else {
        contentWidth = UI_MARGIN + _contentMinimumWidth + UI_MARGIN;
    }

    return QSize(
        std::max(UI_MIN_CONTENT_SIZE, std::max(titleWidth, contentWidth)),
        std::max(UI_MIN_CONTENT_SIZE, UI_TITLE_HEIGHT + UI_MARGIN + _contentMinimumHeight + UI_MARGIN)
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
        if( _status.isClipping ) {
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
        update();
    }
}
