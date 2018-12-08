#include "compwidget.h"

void CompWidget::paint_(QRect rect)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::RenderHint::Antialiasing);

    if( rect.width() >= rect.height() ) rect.setWidth(rect.height());
    else if(rect.width() < rect.height() ) rect.setHeight(rect.width());

    rect.adjust(5, 5, -5, -5);

    // BASE
    drawArc(painter, rect, Qt::darkGray, 8.0, 0.0, 1.0);

    // LEVEL
    drawArc(painter, rect, Qt::red, 8.0, 1.0, _status.params[6].value);

    // METER
    drawArc(painter, rect.adjusted(8, 8, -8, -8), Qt::white, 4.0, 0.0, fmax(1.0 + _status.params[7].value / 100.0, 0.0));

    // TRESHOLD
    if( _status.params[5].value ) {  // Gate
        drawShaft(painter, rect.adjusted(8, 8, -8, -8), Qt::red, 4.0, 1.0 + _status.params[3].value / 100.0);
    }
    else {
        drawShaft(painter, rect.adjusted(8, 8, -8, -8), Qt::white, 4.0, 1.0 + _status.params[3].value / 100.0);
    }

    // RATIO
    rect.adjust(-5, -5, 5, 5);
    drawShaft(painter, rect, Qt::red, 4.0, 1.0 / _status.params[2].value, 0.85);
}
