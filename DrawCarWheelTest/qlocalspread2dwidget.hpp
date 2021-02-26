#pragma once
#include <QWidget>

class QImage;
class QLocalSpread2DWidget : public QWidget {
	Q_OBJECT

public:
	QLocalSpread2DWidget(QWidget * parent = Q_NULLPTR);
	~QLocalSpread2DWidget();
protected:
	void paintEvent(QPaintEvent *e) override;
private:
	void LoadSpreadImage();
	QImage *spread_image;
};
