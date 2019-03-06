#ifndef LEVELMETERWIDGET_H
#define LEVELMETERWIDGET_H

#include "shared/uihelpers.h"
#include "ui/abstractwidget.h"
#include "audio/levelmeter.h"


class LevelMeterWidget : public AbstractWidget
{
public:
    explicit LevelMeterWidget(QWidget *parent = nullptr) :
        AbstractWidget(parent) {
        _drawMeter = false;
        _contentMinimumWidth = 16 + UI_LEVEL_HANDLE_SIZE;
        _parameterFormats[2] = "%1 dB";
    }

private:
    void paint_(QRect rect) override;
};

#endif // LEVELMETERWIDGET_H
