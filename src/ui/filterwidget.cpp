#include "filterwidget.h"


void FilterWidget::paint_(QRect rect, void* status)
{
    FilterStatus* status_ = (FilterStatus*)status;
    QPainter painter(this);

    painter.setBrush(Qt::white);
    painter.setPen(Qt::white);
    painter.drawText(rect, Qt::AlignTop, QString("freq : ") + QString::number(status_->cutoff));
    painter.drawText(rect, Qt::AlignTop, QString("\nreso : ") + QString::number(status_->resonance));
    painter.drawText(rect, Qt::AlignTop, QString("\n\nfeed : ") + QString::number(status_->feedback));
}
