#include "filterwidget.h"


void FilterWidget::paint_(QRect rect, const Status status)
{
    QPainter painter(this);

    painter.setBrush(Qt::white);
    painter.setPen(Qt::white);
    painter.drawText(rect, Qt::AlignTop, QString("freq : ") + QString::number(status.at("cutoff")));
    painter.drawText(rect, Qt::AlignTop, QString("\nreso : ") + QString::number(status.at("resonance")));
    painter.drawText(rect, Qt::AlignTop, QString("\n\nfeed : ") + QString::number(status.at("feedback")));
}
