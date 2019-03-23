#include "djdeckwidget.h"


void DjDeckWidget::_drawPeaks()
{
    for( int trackIndex = 0; trackIndex < _trackBank->audioFileCount(); trackIndex++ )
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
    painter.setFont(_font);
    painter.drawPixmap(rect.x(), rect.y(), _peaks.at(_status.params[2].value));

    // WAVEFORM
    painter.setPen(Qt::white);
    painter.drawLine(
        rect.left() + _status.params[5].value * PEAK_IMAGE_WIDTH, rect.top(),
        rect.left() + _status.params[5].value * PEAK_IMAGE_WIDTH, rect.top() + PEAK_IMAGE_HEIGHT
    );

    // LIST
    for( int row = 0; row < UI_DECK_ROW_COUNT; row++ )
    {
        int audioFileIndex = row + int(_status.params[2].value) - UI_DECK_SELECTED_ROW;
        if( audioFileIndex < 0 || audioFileIndex >= _trackBank->audioFileCount())
            continue;

        QRect rectText(
            rect.left(),
            rect.top() + row * UI_DECK_ROW_HEIGHT + PEAK_IMAGE_HEIGHT + UI_MARGIN,
            PEAK_IMAGE_WIDTH,
            UI_DECK_ROW_HEIGHT
        );
        if( row == UI_DECK_SELECTED_ROW )
        {
            painter.setBrush(Qt::white);
            painter.setPen(Qt::black);
            painter.drawRect(rectText);
        }
        else
        {
            painter.setBrush(Qt::transparent);
            painter.setPen(Qt::white);
        }
        painter.drawText(
            rectText,
            QString::fromStdString(_trackBank->audioFileInfos(audioFileIndex).name)
        );
    }

}

QString DjDeckWidget::formatParameter(int paramId)
{
    if( paramId == 2) { // Track
        return QString::fromStdString(_trackBank->audioFileInfos(_status.params[2].value).name);
    }

    return AbstractWidget::formatParameter(paramId);
}
