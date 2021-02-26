#include "qtwoopenglwidget.hpp"
#include "qworldopenglwidget.hpp"
#include "qlocalopenglwidget.hpp"
#include "qlayout.h"
QTwoOpenglWidget::QTwoOpenglWidget(QWidget * parent) : QWidget(parent) {
	//this->worldWidget = new QWorldOpenglWidget(this);
	this->localWidget = new QLocalOpenGLWidget(this);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	//mainLayout->addWidget(worldWidget);
	mainLayout->addWidget(localWidget);

	this->setLayout(mainLayout);
	this->resize(600, 600);
}

QTwoOpenglWidget::~QTwoOpenglWidget() {
	
}
