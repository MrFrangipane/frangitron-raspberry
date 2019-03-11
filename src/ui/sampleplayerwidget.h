#ifndef SAMPLEPLAYERWIDGET_H
#define SAMPLEPLAYERWIDGET_H


#include "ui/abstractwidget.h"
#include "audio/sampleplayer.h"


class SamplePlayerWidget : public AbstractWidget
{
public:
    explicit SamplePlayerWidget(QWidget *parent = nullptr) :
        AbstractWidget(parent) {
        _contentMinimumWidth = 70;
        _contentMinimumHeight = _contentMinimumWidth;
    }

private:
    void paint_(QRect rect) override;
};

#endif // SAMPLEPLAYERWIDGET_H
