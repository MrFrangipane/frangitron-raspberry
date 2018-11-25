#ifndef ABSTRACTWIDGET_H
#define ABSTRACTWIDGET_H

#include <QObject>
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
    void update_(void *status_);
    void desselect() { _selected = false; QWidget::update(); }

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void selectedChanged(bool isSelected);

private:
    void* _status;
    bool _selected = false;
    virtual void paint_(QRect rect, void* status) { }
};

#endif // ABSTRACTWIDGET_H
