#ifndef TEMPLATEMODULEWIDGET_H
#define TEMPLATEMODULEWIDGET_H


#include "ui/abstractwidget.h"
#include "audio/templatemodule.h"


class TemplateModuleWidget : public AbstractWidget
{
public:
    explicit TemplateModuleWidget(QWidget *parent = nullptr) :
        AbstractWidget(parent) {
        _contentMinimumWidth = 70;
        _contentMinimumHeight = _contentMinimumWidth;
    }

private:
    void paint_(QRect rect) override;
};

#endif // TEMPLATEMODULEWIDGET_H
