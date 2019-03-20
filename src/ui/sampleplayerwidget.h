#ifndef SAMPLEPLAYERWIDGET_H
#define SAMPLEPLAYERWIDGET_H


#include "ui/abstractwidget.h"
#include "audio/_samplebank.h"
#include "audio/sampleplayer.h"


class SamplePlayerWidget : public AbstractWidget
{
public:
    explicit SamplePlayerWidget(const SampleBank * const sampleBank,  QWidget *parent = nullptr) :
        AbstractWidget(parent) {
        _contentMinimumWidth = 70;
        _contentMinimumHeight = _contentMinimumWidth;
    }

private:
    void paint_(QRect rect) override;
};

#endif // SAMPLEPLAYERWIDGET_H
