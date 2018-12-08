#ifndef ABSTRACTWIDGET_H
#define ABSTRACTWIDGET_H

#include <atomic>
#include <cmath>
#include "typedefs.h"
#include "ui/helpers.h"
#include <QObject>
#include <QApplication>
#include <QFontMetrics>
#include <QSize>
#include <QString>
#include <QRect>
#include <QWidget>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

class AbstractWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractWidget(QWidget *parent = nullptr) : QWidget(parent) { }
    void update_(const ModuleStatus status);
    void desselect() { _selected = false; QWidget::update(); }
    bool isSelected() { return _selected; }

protected:
    ModuleStatus _status;
    int _minimumWidth = -1;
    int _minimumHeight = -1;
    bool _drawMeter = true;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    QSize minimumSizeHint() const;

signals:
    void selectedChanged(bool isSelected);

private:
    bool _selected = false;
    virtual void paint_(QRect /*rect*/) { }
    std::atomic<bool> _isWritingStatus{false};
    std::atomic<bool> _isReadingStatus{false};
};

#endif // ABSTRACTWIDGET_H
