#include "levelmeterwidget.h"

void LevelMeterWidget::paint_(QRect rect)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);

    // clipping
    if( _status.params[5].value > 0 ) {
        painter.setBrush(Qt::red);
    } else {
        painter.setBrush(Qt::white);
    }

    // rmsL
    painter.drawRect(rect.adjusted(0, -_status.params[0].value * UI_LEVEL_MAGIC_COEFF * rect.height(), -rect.width() / 2, 0));
    // rmsR
    painter.drawRect(rect.adjusted(rect.width() / 2, -_status.params[1].value * UI_LEVEL_MAGIC_COEFF * rect.height(), 0, 0));
}
