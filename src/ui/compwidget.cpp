#include "compwidget.h"

void CompWidget::paint_(QRect rect)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::RenderHint::Antialiasing);

    QRect rect_ = rect;

    // "SQUARE" (with aspect ratio correction)
    if( (float)rect_.width() >= (float)rect_.height() * UI_ASPECT_RATIO) rect_.setWidth((int)((float)rect_.height() * UI_ASPECT_RATIO));
    else if((float)rect_.width() < (float)rect_.height() * UI_ASPECT_RATIO ) rect_.setHeight((int)((float)rect_.width() / UI_ASPECT_RATIO));
    rect_.moveCenter(rect.center());

    QRect arcRect = rect_.adjusted(2, 2, -2, -2);

    // BASE
    drawArc(painter, arcRect, _darkGrey, UI_ARC_WIDTH, 0.0, 1.0);

    // LEVEL
    drawArc(painter, arcRect, Qt::red, UI_ARC_WIDTH, 1.0, 1.0 - (1.0 / _status.params[6].value - 1.0) / 9.0);

    // METER IN
    drawArc(painter, arcRect.adjusted(UI_ARC_WIDTH, UI_ARC_WIDTH, -UI_ARC_WIDTH, -UI_ARC_WIDTH), Qt::white, UI_ARC_WIDTH, 0.0, fmax(1.0 + _status.params[7].value / 100.0, 0.0));

    // TRESHOLD
    if( _status.params[5].value ) {  // Gate
        drawShaft(painter, rect_.adjusted(UI_ARC_WIDTH, UI_ARC_WIDTH, -UI_ARC_WIDTH, -UI_ARC_WIDTH), Qt::red, UI_SHAFT_WIDTH, 1.0 + _status.params[3].value / 100.0);
    }
    else {
        drawShaft(painter, rect_.adjusted(UI_ARC_WIDTH, UI_ARC_WIDTH, -UI_ARC_WIDTH, -UI_ARC_WIDTH), Qt::white, UI_SHAFT_WIDTH, 1.0 + _status.params[3].value / 100.0);
    }

    // RATIO
    drawShaft(painter, rect_, Qt::red, UI_SHAFT_WIDTH, 1.0 - (_status.params[2].value - 1.0) / 9.0, 0.85);
}

QString CompWidget::formatParameter(int paramId)
{
    if( paramId < 2) { // Attack Release
        return _parameterFormats[paramId].arg(QString::number((int)(_status.params[paramId].value * 1000)));
    }
    else if( paramId == 2) { // Ratio
        return _parameterFormats[paramId].arg(QString::number(_status.params[paramId].value));
    }

    return AbstractWidget::formatParameter(paramId);
}
