#ifndef LEVELMETERWIDGET_H
#define LEVELMETERWIDGET_H

#include "ui/abstractwidget.h"
#include "audio/levelmeter.h"

class LevelMeterWidget : public AbstractWidget
{
public:
    explicit LevelMeterWidget(QWidget *parent = nullptr) :
        AbstractWidget(parent) {
        _drawMeter = false;
    }

private:
    void paint_(QRect rect) override;
};

#endif // LEVELMETERWIDGET_H
