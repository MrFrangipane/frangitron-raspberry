#ifndef DJDECKWIDGET_H
#define DJDECKWIDGET_H

#include "ui/abstractwidget.h"
#include "audio/djdeck.h"
#include "audio/_djtrackbank.h"


class DjDeckWidget : public AbstractWidget
{
public:
    explicit DjDeckWidget(DjTrackBank * const trackBank, QWidget *parent = nullptr) :
        AbstractWidget(parent),
        _trackBank(trackBank)
    {
        _contentMinimumWidth = 280;
        _contentMinimumHeight = _contentMinimumWidth;
    }

private:
    void paint_(QRect rect) override;
    DjTrackBank * const _trackBank;
};

#endif // DJDECKWIDGET_H
