#ifndef DJDECKWIDGET_H
#define DJDECKWIDGET_H

#include "ui/abstractwidget.h"
#include "audio/djdeck.h"


class DjDeckWidget : public AbstractWidget
{
public:
    explicit DjDeckWidget(QWidget *parent = nullptr) :
        AbstractWidget(parent) {
        _contentMinimumWidth = 70;
        _contentMinimumHeight = _contentMinimumWidth;
    }

private:
    void paint_(QRect rect) override;
};

#endif // DJDECKWIDGET_H
