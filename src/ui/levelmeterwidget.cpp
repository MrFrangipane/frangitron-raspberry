#include "levelmeterwidget.h"
#include <iostream>


void LevelMeterWidget::paint_(QRect rect)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);

    // METERS
    QRect meterRect = rect.adjusted(0, 0, -UI_LEVEL_HANDLE_SIZE, 0);

    float left = std::fmin(meterRect.height(), -_status.params[5].value * UI_LEVEL_MAGIC_COEFF * meterRect.height());
    float right = std::fmin(meterRect.height(), -_status.params[6].value * UI_LEVEL_MAGIC_COEFF * meterRect.height());

    QColor color;
    if( _status.params[9].value > 0 ) { // clipping
        color = Qt::red;
    } else {
        color = Qt::white;
    }

    fillRect(painter, meterRect.adjusted(0, left, -meterRect.width() / 2, 0), color);
    fillRect(painter, meterRect.adjusted(meterRect.width() / 2, right, 0, 0), color);

    // LEVEL HANDLE
    painter.setRenderHint(QPainter::Antialiasing, true);

    float level = (_status.params[2].value - 6.0) / 66.0;
    float top = rect.top() - level * (rect.height() - UI_LEVEL_HANDLE_SIZE);
    QRect levelRect = QRect(meterRect.right(), top, UI_LEVEL_HANDLE_SIZE, UI_LEVEL_HANDLE_SIZE);

    if( _status.params[10].value > 0 ) { // locked
        drawTriangle(painter, levelRect, Qt::darkGray, UI_LINE_WIDTH);
    } else {
        fillTriangle(painter, levelRect, Qt::white);
    }
}
