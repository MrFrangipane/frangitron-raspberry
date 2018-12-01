#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include "ui/abstractwidget.h"
#include "audio/filter.h"

class FilterWidget : public AbstractWidget
{
public:
    explicit FilterWidget(QWidget *parent = nullptr) : AbstractWidget(parent) { }

private:
    void paint_(QRect rect, Status const status) override;
};

#endif // FILTERWIDGET_H
