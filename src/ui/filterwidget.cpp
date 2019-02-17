#include "filterwidget.h"


void FilterWidget::paint_(QRect rect)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::RenderHint::Antialiasing);

    QRect rect_ = rect;

    // "SQUARE" (with aspect ratio correction)
    if( (float)rect_.width() >= (float)rect_.height() * UI_ASPECT_RATIO) rect_.setWidth((int)((float)rect_.height() * UI_ASPECT_RATIO));
    else if((float)rect_.width() < (float)rect_.height() * UI_ASPECT_RATIO ) rect_.setHeight((int)((float)rect_.width() / UI_ASPECT_RATIO));
    rect_.adjust(10, 10, -10, -10);
    rect_.moveCenter(rect.center());

    // BASE
    drawArc(painter, rect_, Qt::darkGray, 8.0, 0.0, 1.0);

    // CUTOFF
    drawShaft(painter, rect_, Qt::darkGray, UI_SHAFT_WIDTH, _status.params[1].value / 2.0 + 0.5);

    if( _status.params[1].value > 0) {
        drawArc(painter, rect_, Qt::white, 8.0, _status.params[1].value, 1.0);
    }
    else {
        drawArc(painter, rect_, Qt::white, 8.0, 0.0, 1.0 + _status.params[1].value);
    }

    // Resonance
    fillCircle(painter, rect_, Qt::white, lerp(0.2, 0.8, _status.params[3].value));
}
