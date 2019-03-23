#include "djdeckwidget.h"


void DjDeckWidget::_drawPeaks()
{
    for( int trackIndex = 0; trackIndex < _trackBank->trackCount(); trackIndex++ )
    {
        QPixmap pixmap(PEAK_IMAGE_WIDTH, PEAK_IMAGE_HEIGHT);
        pixmap.fill(Qt::transparent);

        QPainter painter(&pixmap);
        painter.setPen(_lightGrey);

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
    painter.drawPixmap(rect.x(), rect.y(), _peaks.at(_status.params[2].value));

    painter.setPen(Qt::white);
    painter.drawLine(
        rect.left() + _status.params[5].value * PEAK_IMAGE_WIDTH, rect.top(),
        rect.left() + _status.params[5].value * PEAK_IMAGE_WIDTH, rect.top() + PEAK_IMAGE_HEIGHT
    );
}

QString DjDeckWidget::formatParameter(int paramId)
{
    if( paramId == 2) { // Track
        return QString::fromStdString(_trackBank->audioFileInfos(_status.params[2].value).name);
    }

    return AbstractWidget::formatParameter(paramId);
}
