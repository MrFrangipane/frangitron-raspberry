#include "levelmeterwidget.h"

void LevelMeterWidget::paint_(QRect rect)
{
    QPainter painter(this);

    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    // rmsL
    painter.drawRect(rect.adjusted(0, -_status.params[0].value * UI_LEVEL_MAGIC_COEFF * rect.height(), -rect.width() / 2, 0));
    // rmsR
    painter.drawRect(rect.adjusted(rect.width() / 2, -_status.params[1].value * UI_LEVEL_MAGIC_COEFF * rect.height(), 0, 0));
}
