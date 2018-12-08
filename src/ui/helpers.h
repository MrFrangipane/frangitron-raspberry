#ifndef HELPERS_H
#define HELPERS_H

#include <cmath>
#include <QRect>
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QBrush>

inline void drawArc(QPainter& painter, QRect rect, QColor color, float width, float start, float end) {
    QBrush brushBackup = painter.brush();
    QPen penBackup = painter.pen();
    QPen penArc = painter.pen();
    QRect rectAdjusted = rect.adjusted(width / 2, width / 2, -width / 2, -width / 2);

    penArc.setCapStyle(Qt::FlatCap);
    penArc.setStyle(Qt::SolidLine);
    penArc.setColor(color);
    penArc.setWidth(width);

    painter.setBrush(Qt::NoBrush);
    painter.setPen(penArc);
    painter.drawArc(
        rectAdjusted,
        ((-start * 300) - 120) * 16,
        (start - end) * 300 * 16
    );

    painter.setBrush(brushBackup);
    painter.setPen(penBackup);
}

inline void drawShaft(QPainter& painter, QRect rect, QColor color, float width, float value, float start = 0.0, float end = 1.0) {
    QBrush brushBackup = painter.brush();
    QPen penBackup = painter.pen();
    QPen penArc = painter.pen();
    QRect rectAdjusted = rect.adjusted(width / 2, width / 2, -width / 2, -width / 2);

    float angle = M_PI * (5.0 * value + 2.0) / 3.0;

    penArc.setCapStyle(Qt::RoundCap);
    penArc.setStyle(Qt::SolidLine);
    penArc.setColor(color);
    penArc.setWidth(width);

    painter.setBrush(Qt::NoBrush);
    painter.setPen(penArc);
    painter.drawLine(
        rectAdjusted.center().x() + rectAdjusted.width() * std::cos(angle) * 0.5 * start,
        rectAdjusted.center().y() + rectAdjusted.height() * std::sin(angle) * 0.5 * start,
        rectAdjusted.center().x() + rectAdjusted.width() * std::cos(angle) * 0.5 * end,
        rectAdjusted.center().y() + rectAdjusted.height() * std::sin(angle) * 0.5 * end
    );

    painter.setBrush(brushBackup);
    painter.setPen(penBackup);
}

inline void fillRect(QPainter& painter, QRect rect, QColor color) {
    QBrush brushBackup = painter.brush();
    QPen penBackup = painter.pen();

    painter.setBrush(color);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect);

    painter.setBrush(brushBackup);
    painter.setPen(penBackup);
}

inline void fillCircle(QPainter& painter, QRect rect, QColor color, float radius) {
    QBrush brushBackup = painter.brush();
    QPen penBackup = painter.pen();

    painter.setBrush(color);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(rect.center(), (int)((float)rect.width() * radius * 0.5), (int)((float)rect.height() * radius * 0.5));

    painter.setBrush(brushBackup);
    painter.setPen(penBackup);
}

#endif // HELPERS_H
