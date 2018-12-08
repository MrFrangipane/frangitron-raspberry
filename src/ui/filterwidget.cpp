#include "filterwidget.h"


void FilterWidget::paint_(QRect rect)
{
    QPainter painter(this);

    painter.setBrush(Qt::white);
    painter.setPen(Qt::white);
    // Cutoff
    painter.drawText(rect, Qt::AlignTop, QString("freq : ") + QString::number(_status.params[0].value));
    // Resonance
    painter.drawText(rect, Qt::AlignTop, QString("\nreso : ") + QString::number(_status.params[1].value));
}
