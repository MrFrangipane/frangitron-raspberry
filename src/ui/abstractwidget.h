#ifndef ABSTRACTWIDGET_H
#define ABSTRACTWIDGET_H

#include <cmath>
#include "shared/constants.h"
#include "shared/typedefs.h"
#include "shared/structures.h"
#include "shared/uihelpers.h"
#include <QObject>
#include <QColor>
#include <QString>
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
    virtual QString formatParameter(int paramId) {
        return _parameterFormats[paramId].arg(QString::number(_status.params[paramId].value, 'f', 2));
    }

protected:
    ModuleStatus _status;
    int _minimumWidth = -1;
    int _minimumHeight = -1;
    bool _drawMeter = true;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    QSize minimumSizeHint() const;
    virtual void paint_(QRect /*rect*/) { }
    QString _parameterFormats[5] = {"%1", "%1", "%1", "%1", "%1"};
    QColor _darkGrey = QColor(60, 60, 60);

signals:
    void selectedChanged(bool isSelected);

private:
    bool _selected = false;
};

#endif // ABSTRACTWIDGET_H
