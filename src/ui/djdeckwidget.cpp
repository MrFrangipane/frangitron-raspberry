#include "djdeckwidget.h"


void DjDeckWidget::_drawPeaks()
{
    for( int fileIndex = 0; fileIndex < _trackBank->audioFileCount(); fileIndex++ )
    {
        AudioFileInfos infos = _trackBank->audioFileInfos(fileIndex);

        QPixmap pixmap(PEAK_IMAGE_WIDTH, PEAK_IMAGE_HEIGHT);
        pixmap.fill(Qt::transparent);

        QPainter painter(&pixmap);
        painter.setPen(_lightGrey);

        for( int i = 0; i < PEAK_IMAGE_WIDTH; i++ )
        {
            painter.drawLine(
                i,
                PEAK_IMAGE_HEIGHT * 0.5 * (1 - infos.peaks.at(i)),
                i,
                PEAK_IMAGE_HEIGHT * 0.5 * (1 + infos.peaks.at(i))
            );
        }

        for( int i = 0; i < infos.cueCount; i++ )
        {
            float pos = (float)infos.cues[i].position / (float)infos.frameCount;
            pos = pos * PEAK_IMAGE_WIDTH;
            painter.drawLine(
                pos, 0,
                pos, PEAK_IMAGE_HEIGHT
            );
        }

        _peaks.append(pixmap);
    }
}

void DjDeckWidget::paint_(QRect rect)
{
    QPainter painter(this);
    painter.setFont(_font);
    if( _status.params[2].value >= 0 )
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

        if( audioFileIndex < 0 || audioFileIndex >= _trackBank->audioFileCount())
            continue;

        painter.drawText(
            rectText,
            QString::fromStdString(_trackBank->audioFileInfos(audioFileIndex).name)
        );
    }

}

QString DjDeckWidget::formatParameter(int paramId)
{
    if( paramId == 2) { // Track
        if( _status.params[2].value < 0 )
            return QString("");
        else
            return QString::fromStdString(_trackBank->audioFileInfos(_status.params[2].value).name);
    }

    return AbstractWidget::formatParameter(paramId);
}
