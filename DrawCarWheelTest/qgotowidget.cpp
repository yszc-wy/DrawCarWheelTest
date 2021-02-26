#include "qgotowidget.h"
#include <QWidget>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLayout>
QGoToWidget::QGoToWidget(QWidget *parent) : QWidget(parent)
{
    this->widthLocateSpinBox=new QDoubleSpinBox(this);
    this->widthLocateSpinBox->setDecimals(2);
    this->widthLocateSpinBox->setRange(50,100);
    this->widthLocateSpinBox->setValue(50);

    this->sectionLocateSpinBox=new QDoubleSpinBox(this);
    this->sectionLocateSpinBox->setDecimals(3);
    this->sectionLocateSpinBox->setRange(0,360);
    this->sectionLocateSpinBox->setValue(0);

    this->widthLocateLabel=new QLabel(tr("宽度定位："),this);
    this->sectionLocateLabel=new QLabel(tr("角度定位："),this);

    this->widthUnitLabel=new QLabel(tr("mm"),this);
    this->sectionUnitLabel=new QLabel(tr("mm"),this);

    QHBoxLayout * widthLocateLayout=new QHBoxLayout;
    widthLocateLayout->addWidget(this->widthLocateLabel);
    widthLocateLayout->addWidget(this->widthLocateSpinBox);
    widthLocateLayout->addWidget(this->widthUnitLabel);

    QHBoxLayout * sectionLocateLayout=new QHBoxLayout;
    sectionLocateLayout->addWidget(this->sectionLocateLabel);
    sectionLocateLayout->addWidget(this->sectionLocateSpinBox);
    sectionLocateLayout->addWidget(this->sectionUnitLabel);

    QVBoxLayout *mainLayout=new QVBoxLayout;
    mainLayout->addLayout(widthLocateLayout);
    mainLayout->addLayout(sectionLocateLayout);

    this->setLayout(mainLayout);

    connect(this->sectionLocateSpinBox,SIGNAL(valueChanged(double)),this,SIGNAL(sectionLocateSpinBoxValueChanged(double)));
    connect(this->widthLocateSpinBox,SIGNAL(valueChanged(double)),this,SIGNAL(widthLocateSpinBoxValueChanged(double)));
}

void QGoToWidget::setWidthLocateValue(float value)//?
{
    this->widthLocateSpinBox->setValue(value);
}
void QGoToWidget::setSectionLocateValue(float value)//?
{
    this->sectionLocateSpinBox->setValue(value);
}
void QGoToWidget::setWidthLocateRange(double start,double end)
{
    this->widthLocateSpinBox->setRange(start,end);
}
