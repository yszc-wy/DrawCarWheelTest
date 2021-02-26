#pragma once
class Range
{
public:
	Range() :start(0), end(0) {}
	Range(float start, float end) :start(start), end(end) {}
	float GetStart() const
	{
		return start;
	}
	float GetEnd() const
	{
		return end;
	}
	float GetMin() const
	{
		return start < end ? start : end;
	}
	float GetMax() const
	{
		return start > end ? start : end;
	}
	float GetDifference() const
	{
		return GetMax() - GetMin();
	}
	void SetRange(float start, float end)
	{
		this->start = start;
		this->end = end;
	}

private:
	float start;
	float end;
};