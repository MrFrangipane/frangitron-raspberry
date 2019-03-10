#include "kickwidget.h"


void KickWidget::paint_(QRect rect)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::RenderHint::Antialiasing);

    fillCircle(painter, rect, Qt::white, _status.params[5].value);

    //fillRect(painter, rect.adjusted((1.0 - _status.params[6].value) * rect.width(), rect.height() / 2, 0, 0), Qt::red);
}
