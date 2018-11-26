#include "filterwidget.h".h"

void FilterWidget::paint_(QRect rect, void* status)
{
    FilterStatus* status_ = (FilterStatus*)status;
    QPainter painter(this);

    painter.setBrush(Qt::white);
    painter.setPen(Qt::white);
    painter.drawText(rect, Qt::AlignCenter, QString::number(status_->cutoff));
}
