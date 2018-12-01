#include "compwidget.h"

void CompWidget::paint_(QRect rect, const Status status)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::RenderHint::Antialiasing);

    // TEXT
    painter.drawText(rect, Qt::AlignBottom, QString::number(1.0 + status.at("threshold") / 100.0));


    if( rect.width() >= rect.height() ) rect.setWidth(rect.height());
    else if(rect.width() < rect.height() ) rect.setHeight(rect.width());

    rect.adjust(5, 5, -5, -5);

    // BASE
    drawArc(painter, rect, Qt::darkGray, 8.0, 0.0, 1.0);

    // METER
    drawArc(painter, rect, Qt::red, 8.0, 1.0, status.at("level"));

    // LEVEL
    drawArc(painter, rect.adjusted(8, 8, -8, -8), Qt::white, 4.0, 0.0, fmax(1.0 + status.at("rms") / 100.0, 0.0));

    // TRESHOLD
    if( status.at("gate") ) {
        drawShaft(painter, rect.adjusted(8, 8, -8, -8), Qt::red, 4.0, 1.0 + status.at("threshold") / 100.0);
    }
    else {
        drawShaft(painter, rect.adjusted(8, 8, -8, -8), Qt::white, 4.0, 1.0 + status.at("threshold") / 100.0);
    }
}
