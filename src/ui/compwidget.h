#ifndef COMPWIDGET_H
#define COMPWIDGET_H

#include "shared/uihelpers.h"
#include "ui/abstractwidget.h"
#include "audio/compressor.h"

class CompWidget : public AbstractWidget
{
public:
    explicit CompWidget(QWidget *parent = nullptr) :
        AbstractWidget(parent) {
        _minimumWidth = 80;
        _minimumHeight = 105;  // 25 px Title
        _parameterFormats[0] = "%1 ms";
        _parameterFormats[1] = "%1 ms";
        _parameterFormats[2] = "1:%1";
        _parameterFormats[3] = "%1 dB";
        _parameterFormats[4] = "%1 dB";
    }
    QString formatParameter(int paramId) override;

private:
    void paint_(QRect rect) override;
};

#endif // COMPWIDGET_H
