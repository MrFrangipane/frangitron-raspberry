#include "filterwidget.h"


void FilterWidget::paint_(QRect rect)
{
    QPainter painter(this);

    painter.setBrush(Qt::white);
    painter.setPen(Qt::white);
    painter.drawText(rect, Qt::AlignTop, QString("freq : ") + QString::number(_status["cutoff"]));
    painter.drawText(rect, Qt::AlignTop, QString("\nreso : ") + QString::number(_status["resonance"]));
}
