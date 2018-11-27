#include "compwidget.h"

void CompWidget::paint_(QRect rect, void* status)
{
    CompressorStatus* status_ = (CompressorStatus*)status;
    QPainter painter(this);
    painter.setRenderHints(QPainter::RenderHint::Antialiasing);
    QPen pen(Qt::darkGray, 8.0, Qt::SolidLine, Qt::FlatCap);

    if( rect.width() >= rect.height() ) rect.setWidth(rect.height());
    else if(rect.width() < rect.height() ) rect.setHeight(rect.width());

    painter.setBrush(Qt::NoBrush);

    // BASE
    painter.setPen(pen);

    painter.drawArc(
        rect.adjusted(13, 13, -13, -13),
        -60 * 16,
        300 * 16
    );

    // METER
    int arcOffset = -120;
    int arcSpan = ((status_->rms / 200.0)) * 300;

    pen.setColor(Qt::white);
    pen.setWidth(6);
    painter.setPen(pen);
    painter.drawArc(
        rect.adjusted(20, 20, -20, -20),
        arcOffset * 16,
        arcSpan * 16
    );

    // LEVEL
    arcOffset = -60;
    arcSpan = (1.0 - status_->level) * 300;

    pen.setColor(Qt::red);
    pen.setWidth(8);
    painter.setPen(pen);

    painter.drawArc(
        rect.adjusted(13, 13, -13, -13),
        arcOffset * 16,
        arcSpan * 16
    );

    // TRESHOLD
    float shaft_angle = M_PI * (0.01 * status_->threshold + 4) / 6;
    pen.setColor(Qt::white);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(4);
    painter.setPen(pen);
    painter.drawLine(
        rect.center().x(),
        rect.center().y(),
        rect.center().x() + (rect.width() - 15) * std::cos(M_PI * shaft_angle) / 2.0,
        rect.center().y() + (rect.height() - 15) * std::sin(M_PI * shaft_angle) / 2.0
    );

}
