#pragma once
namespace DxfReader {
	class Point {
	public:
		Point(double x=0.0f,double y=0.0f):x(x),y(y){}

		double x;
		double y;
	};
}