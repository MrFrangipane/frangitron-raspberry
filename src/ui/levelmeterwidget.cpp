#include "levelmeterwidget.h"

void LevelMeterWidget::paint_(QRect rect, void* status)
{
    if( status == nullptr ) return;
    LevelMeterStatus* status_ = (LevelMeterStatus*)status;
    QPainter painter(this);

    painter.drawText(rect, QString::number(status_->rmsInstantL));
}
