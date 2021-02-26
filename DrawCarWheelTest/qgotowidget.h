#ifndef QGOTOWIDGET_H
#define QGOTOWIDGET_H

#include <QWidget>
class QDoubleSpinBox;
class QLabel;
class QGoToWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QGoToWidget(QWidget *parent = nullptr);

signals:
    void widthLocateSpinBoxValueChanged(double value);
    void sectionLocateSpinBoxValueChanged(double value);
public slots:
    void setWidthLocateValue(float value);//?
    void setSectionLocateValue(float value);//?
    void setWidthLocateRange(double start,double end);
private:
    QDoubleSpinBox *widthLocateSpinBox;
    QDoubleSpinBox *sectionLocateSpinBox;
    QLabel *widthLocateLabel;
    QLabel *sectionLocateLabel;
    QLabel *widthUnitLabel;
    QLabel *sectionUnitLabel;
};

#endif // QGOTOWIDGET_H
