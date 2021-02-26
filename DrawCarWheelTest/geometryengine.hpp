#pragma once
#include <QOpenGLFunctions>
#include <qopenglshaderprogram.h>
#include <qopenglbuffer.h>
#include <qopenglvertexarrayobject.h>
class GeometryEngine : protected QOpenGLFunctions {
public:
	GeometryEngine();
	virtual ~GeometryEngine();

	void drawCubeGeometry(QOpenGLShaderProgram *program);

private:
	void initCubeGeometry();

	QOpenGLBuffer arrayBuf;//VBO
	QOpenGLBuffer indexBuf;//EBO
};
