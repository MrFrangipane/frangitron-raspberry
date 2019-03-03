#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include "ui/abstractwidget.h"
#include "audio/filter.h"

class FilterWidget : public AbstractWidget
{
public:
    explicit FilterWidget(QWidget *parent = nullptr) :
        AbstractWidget(parent) {
        _minimumWidth = 100;
        _minimumHeight = _minimumWidth + 25;  // 25 px Title
    }

private:
    void paint_(QRect rect) override;
};

#endif // FILTERWIDGET_H
