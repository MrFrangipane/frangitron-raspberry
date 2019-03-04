#ifndef KICKWIDGET_H
#define KICKWIDGET_H


#include "ui/abstractwidget.h"
#include "audio/kicksynth.h".h"


class KickWidget : public AbstractWidget
{
public:
    explicit KickWidget(QWidget *parent = nullptr) :
        AbstractWidget(parent) {
        _contentMinimumWidth = 70;
        _contentMinimumHeight = _contentMinimumWidth;
    }

private:
    void paint_(QRect rect) override;
};

#endif // KICKWIDGET_H
