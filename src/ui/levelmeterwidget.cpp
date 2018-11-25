#include "levelmeterwidget.h"

void LevelMeterWidget::paint_(QRect rect, void* status)
{
    LevelMeterStatus* status_ = (LevelMeterStatus*)status;
    QPainter painter(this);

    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect.adjusted(0, -status_->rmsL * 0.0045 * rect.height(), -rect.width() / 2, 0));
    painter.drawRect(rect.adjusted(rect.width() / 2, -status_->rmsR * 0.0045 * rect.height(), 0, 0));
}
