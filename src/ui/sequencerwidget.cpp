#include "sequencerwidget.h"

void SequencerWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    int step = _step % 16;
    int page = (_step / 16) % 4;

    // COMPUTE RECTS
    int cell_width = event->rect().width() / 21;
    int cell_height = event->rect().height();
    int top = (cell_height - cell_width) / 2;

    // STEPS
    for( int i = 0; i < 16; i++ ) {
        QRect cell(
            cell_width * i,
            top,
            cell_width,
            cell_width
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
            cell_width * (i + 16.5),
            top + 3,
            cell_width,
            cell_width - 6
        );

        if( i == page ) fillRect(painter, cell.adjusted(5, 5, -5, -5), Qt::white);
        else drawRect(painter, cell.adjusted(5, 5, -5, -5), Qt::darkGray, 1);
    }
}
