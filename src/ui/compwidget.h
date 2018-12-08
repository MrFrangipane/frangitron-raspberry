#ifndef COMPWIDGET_H
#define COMPWIDGET_H

#include "ui/helpers.h"
#include "ui/abstractwidget.h"
#include "audio/compressor.h"

class CompWidget : public AbstractWidget
{
public:
    explicit CompWidget(QWidget *parent = nullptr) :
        AbstractWidget(parent)
    {
        _minimumWidth = 130;
        _minimumHeight = 130;
    }

private:
    void paint_(QRect rect) override;
};

#endif // COMPWIDGET_H
