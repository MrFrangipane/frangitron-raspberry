#ifndef SEQUENCERWIDGET_H
#define SEQUENCERWIDGET_H

#include <QWidget>
#include <QSize>
#include <QRect>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include "shared/typedefs.h"
#include "shared/uihelpers.h"


class SequencerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SequencerWidget(QWidget *parent = nullptr) : QWidget(parent) { }
    void setStep(int step) { _step = step; update(); }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int _step = 0;

signals:

public slots:
};

#endif // SEQUENCERWIDGET_H
