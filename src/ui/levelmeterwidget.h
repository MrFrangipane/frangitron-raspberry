#ifndef LEVELMETERWIDGET_H
#define LEVELMETERWIDGET_H

#include "abstractwidget.h"
#include "levelmeter.h"

class LevelMeterWidget : public AbstractWidget
{
public:
    explicit LevelMeterWidget(QWidget *parent = nullptr) :
        AbstractWidget(parent)
    {
        _minimumWidth = 60;
    }

private:
    void paint_(QRect rect, void* status) override;
};

#endif // LEVELMETERWIDGET_H
