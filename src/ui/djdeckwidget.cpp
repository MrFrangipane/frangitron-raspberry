#include "djdeckwidget.h"


void DjDeckWidget::paint_(QRect rect)
{
    QPainter painter(this);
//    painter.setRenderHints(QPainter::RenderHint::Antialiasing);

    for( int peakIndex = 0; peakIndex < 280; peakIndex++ )
    {
        drawRect(painter, QRect(peakIndex, 0, 1, rect.height() * _trackBank->peak(0, peakIndex)), Qt::white, 1);
    }

    painter.drawText(
        rect,
        QString::number(_trackBank->peak(0, 0))
    );
}
