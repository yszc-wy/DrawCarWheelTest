#pragma once
#include <QWidget>
class QWorldOpenglWidget;
class QLocalOpenGLWidget;

class QTwoOpenglWidget : public QWidget {
	Q_OBJECT

public:
	QTwoOpenglWidget(QWidget * parent = Q_NULLPTR);
	~QTwoOpenglWidget();

private:
	//QWorldOpenglWidget *worldWidget;
	QLocalOpenGLWidget *localWidget;

};
