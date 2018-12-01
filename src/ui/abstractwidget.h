#ifndef ABSTRACTWIDGET_H
#define ABSTRACTWIDGET_H

#include "typedefs.h"
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
    void update_(const Status status_);
    void desselect() { _selected = false; QWidget::update(); }

protected:
    Status _status;
    int _minimumWidth = -1;
    int _minimumHeight = -1;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    QSize minimumSizeHint() const;

signals:
    void selectedChanged(bool isSelected);

private:
    bool _selected = false;
    virtual void paint_(QRect /*rect*/) { }
};

#endif // ABSTRACTWIDGET_H
