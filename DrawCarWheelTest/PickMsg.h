#pragma once
#include <Wheel.h>
class PickMsg {
public:
	PickMsg(float length_radian, float width) :length_radian(length_radian), width(width) {}
	float GetLength()
	{
		return length_radian;
	}
	float GetWidth()
	{
		return width;
	}
	float GetAngle()
	{
		return 360.0f*length_radian / (2 * Wheel::WheelFlangeTread::kPIR);
	}
	float GetRadian()
	{
		return length_radian;
	}
	void SetLength(float length)
	{
		this->length_radian = length;
	}
	void SetWidth(float width)
	{
		this->width = width;
	}
	void SetRadian(float radian)
	{
		this->length_radian = radian;
	}
private:
	float length_radian;
	float width;
};