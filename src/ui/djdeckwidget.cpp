#include "djdeckwidget.h"

void DjDeckWidget::paint_(QRect rect)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::RenderHint::Antialiasing);

    fillCircle(painter, rect, Qt::white, _status.params[5].value);
}
