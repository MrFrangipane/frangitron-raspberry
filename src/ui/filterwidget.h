#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include "abstractwidget.h"
#include "effects/filter.h"

class FilterWidget : public AbstractWidget
{
public:
    explicit FilterWidget(QWidget *parent = nullptr) : AbstractWidget(parent) { }

private:
    void paint_(QRect rect, void* status) override;
};

#endif // FILTERWIDGET_H