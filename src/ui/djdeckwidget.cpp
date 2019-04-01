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

        painter.setPen(Qt::lightGray);
        for( int i = 0; i < infos.cueCount; i++ )
        {
            float pos = (float)infos.cues[i].position / (float)infos.frameCount;
            pos = pos * PEAK_IMAGE_WIDTH;
            painter.drawLine(
                pos, 0,
                pos, PEAK_IMAGE_HEIGHT
            );
        }

        _peaksPixmaps.append(pixmap);
    }
}

void DjDeckWidget::paint_(QRect rect)
{
    QPainter painter(this);

    // WAVEFORM
    if( _status.params[2].value >= 0 )
        painter.drawPixmap(rect.x(), rect.y(), _peaksPixmaps.at(_status.params[2].value));

    // PLAY CURSOR
    painter.setPen(Qt::white);
    painter.drawLine(
        rect.left() + _status.params[5].value * PEAK_IMAGE_WIDTH, rect.top(),
        rect.left() + _status.params[5].value * PEAK_IMAGE_WIDTH, rect.top() + PEAK_IMAGE_HEIGHT
    );

    // LIST
    painter.setFont(_font);
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

    // REGION
    int selectedAudioFile = int(_status.params[2].value);
    int selectedCue = int(_status.params[3].value);

    if( selectedAudioFile >= 0 )
    {
        AudioFileInfos infos = _trackBank->audioFileInfos(selectedAudioFile);
        int regionWidth = 0;
        int regionLeft = infos.cues[selectedCue].imagePosition;

        if( selectedCue < infos.cueCount - 1)
            regionWidth = infos.cues[selectedCue + 1].imagePosition - regionLeft;
        else
            regionWidth = PEAK_IMAGE_WIDTH - regionLeft;

        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::cyan);
        painter.setOpacity(0.25);
        painter.drawRect(
            rect.x() + regionLeft,
            rect.y(),
            regionWidth,
            PEAK_IMAGE_HEIGHT
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
    else if( paramId == 3 ) { // Cue
        return QString::number(int(_status.params[3].value));
    }

    return AbstractWidget::formatParameter(paramId);
}
