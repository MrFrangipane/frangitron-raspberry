#include "sequencerwidget.h"

void SequencerWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    int step = _step % 16;
    int page = (_step / 16) % 4;

    // COMPUTE RECTS
    int cell_width = event->rect().width() / 22;
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



        if( i == step ) fillCircle(painter, cell, Qt::white, .8);
        else if( i % 4 == 0 ) fillCircle(painter, cell, Qt::lightGray, .6);
        else fillCircle(painter, cell, Qt::darkGray, .4);
    }

    // PAGES
    for( int i = 0; i < 4; i++ ) {
        if( i == page ) painter.setBrush(Qt::white);
        else painter.setBrush(Qt::NoBrush);

        QRect cell(
            cell_width * (i + 17),
            top,
            cell_width,
            cell_width
        );

        if( i == page ) fillCircle(painter, cell, Qt::white, .8);
        else fillCircle(painter, cell, Qt::darkGray, .5);
    }
}
