#include "sequencerwidget.h"

void SequencerWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    int step = _step % 16;
    int page = (_step / 16) % 4;

    // COMPUTE RECTS
    int cellWidth = event->rect().width() / 21;
    int cellHeight = event->rect().height();
    int top = (cellHeight - cellWidth) / 2;

    // STEPS
    for( int i = 0; i < 16; i++ ) {
        QRect cell(
            cellWidth * i,
            top,
            cellWidth,
            cellWidth
        );

        if( i == step && i % 4 == 0 ) fillCircle(painter, cell, Qt::white, .8);
        else if( i == step && i % 4 != 0 ) fillCircle(painter, cell, Qt::white, .6);
        else if( i % 4 == 0 ) drawCircle(painter, cell, Qt::white, .6, 2);
        else drawCircle(painter, cell, Qt::darkGray, .4, 2);
    }

    // PAGES
    painter.setRenderHint(QPainter::Antialiasing, false);
    for( int i = 0; i < 4; i++ ) {
        if( i == page ) painter.setBrush(Qt::white);
        else painter.setBrush(Qt::NoBrush);

        QRect cell(
            cellWidth * (i + 16.5),
            top + 3,
            cellWidth,
            cellWidth - 6
        );

        if( i == page ) fillRect(painter, cell.adjusted(5, 5, -5, -5), Qt::white);
        else drawRect(painter, cell.adjusted(5, 5, -5, -5), Qt::darkGray, 1);
    }
}
