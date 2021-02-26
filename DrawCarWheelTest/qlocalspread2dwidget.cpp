#include "qlocalspread2dwidget.hpp"
#include <qpainter.h>
QLocalSpread2DWidget::QLocalSpread2DWidget(QWidget * parent) : QWidget(parent) {
	
}

QLocalSpread2DWidget::~QLocalSpread2DWidget() {
	
}
void QLocalSpread2DWidget::LoadSpreadImage()
{

}
void QLocalSpread2DWidget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	painter.setWindow(0, 0, 600, 135);

	painter.setRenderHint(QPainter::Antialiasing, true);//复杂，耗时间

}