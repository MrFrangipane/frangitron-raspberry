#ifndef PARAMSLIDER_H
#define PARAMSLIDER_H

#include <QSlider>


class ParamSlider : public QSlider
{
    Q_OBJECT
public:
    explicit ParamSlider(QWidget *parent = nullptr) : QSlider(parent) { }
};

#endif // PARAMSLIDER_H
