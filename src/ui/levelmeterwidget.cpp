#include "levelmeterwidget.h"

void LevelMeterWidget::paint_(QRect rect)
{
    QPainter painter(this);

    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect.adjusted(0, -_status["rmsL"] * 0.0045 * rect.height(), -rect.width() / 2, 0));
    painter.drawRect(rect.adjusted(rect.width() / 2, -_status["rmsR"] * 0.0045 * rect.height(), 0, 0));

    if( _status["rmsR"] == 0 ) {
        int you_got_a_breakpoint = 1;
    }
}
