#include "djdeckwidget.h"


void DjDeckWidget::_drawPeaks()
{
    for( int trackIndex = 0; trackIndex < _trackBank->trackCount(); trackIndex++ )
    {
        QPixmap pixmap(PEAK_IMAGE_WIDTH, PEAK_IMAGE_HEIGHT);
        pixmap.fill(Qt::transparent);

        QPainter painter(&pixmap);
        painter.setPen(Qt::white);

        for( int i = 0; i < PEAK_IMAGE_WIDTH; i++ )
        {
            painter.drawLine(
                i,
                PEAK_IMAGE_HEIGHT * 0.5 * (1 - _trackBank->peak(trackIndex, i)),
                i,
                PEAK_IMAGE_HEIGHT * 0.5 * (1 + _trackBank->peak(trackIndex, i))
            );
        }

        _peaks.append(pixmap);
    }
}

void DjDeckWidget::paint_(QRect rect)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawPixmap(rect.x(), rect.y(), _peaks.at(_status.params[2].value));
}

QString DjDeckWidget::formatParameter(int paramId)
{
    if( paramId == 2) { // Track
        return QString::fromStdString(_trackBank->trackInfos(_status.params[2].value).name);
    }

    return AbstractWidget::formatParameter(paramId);
}
