#ifndef DJDECKWIDGET_H
#define DJDECKWIDGET_H

#include <QList>
#include <QPixmap>
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
        _contentMinimumWidth = PEAK_IMAGE_WIDTH;
        _contentMinimumHeight = PEAK_IMAGE_HEIGHT;
        _drawPeaks();
    }
    QString formatParameter(int paramId) override;

private:
    void _drawPeaks();
    void paint_(QRect rect) override;
    QList<QPixmap> _peaks;
    DjTrackBank * const _trackBank;
};

#endif // DJDECKWIDGET_H
