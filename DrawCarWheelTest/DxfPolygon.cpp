#include "DxfPolygon.h"
#include <iostream>
#include <string>
namespace DxfReader {
	void Polygon::Show() const
	{
		this->p_ipolygon->ShowType();
		std::cout << "Direction:"<<this->direction << std::endl;
		std::cout << "StartPoint: ";
		this->p_ipolygon->GetStartPoint().Show();
		std::cout << " EndPoint: ";
		this->p_ipolygon->GetEndPoint().Show();
		std::cout << std::endl;
	}
	std::string Polygon::Get_Log() const
	{
		std::string log;
		log += this->p_ipolygon->GetType() + ":\n";
		log += "Direction:" + std::to_string(this->direction) + " StartPoint:" + this->p_ipolygon->GetStartPoint().Get_Log() + " EndPoint:" + this->p_ipolygon->GetEndPoint().Get_Log() + "\n";
		return log;
	}
	Polygon::Polygon() :
		p_ipolygon(nullptr), direction(Direction::Forward)
	{
	}
	Polygon::Polygon(const Polygon& polygon)
	{
		this->p_ipolygon = polygon.p_ipolygon->clone();
		this->direction = polygon.direction;
		//LoadSEPoint();
	}
	Polygon::Polygon(const std::shared_ptr<IPolygon>& p_ipolygon, Direction direction) :
		p_ipolygon(p_ipolygon), direction(direction)
	{
		//LoadSEPoint();
	}
	Polygon& Polygon::operator=(const Polygon& polygon)//wtf
	{
		this->p_ipolygon = polygon.p_ipolygon->clone();
		this->direction = polygon.direction;
		//LoadSEPoint();
		return *this;
	}
	const std::shared_ptr<IPolygon>& Polygon::get_p_ipolygon() const
	{
		return this->p_ipolygon;
	}
	//void Polygon::LoadSEPoint()
	//{
	//	if (p_ipolygon == nullptr)
	//	{
	//		
	//<< "Warning::Polygon::LoadSEPoint::visit null ptr" << std::endl;
	//		return;
	//	}
	//	start_point = p_ipolygon->GetStartPoint();
	//	end_point = p_ipolygon->GetEndPoint();
	//}
	//Point Polygon::get_start_point() const
	//{
	//	return this->start_point;
	//}
	//Point Polygon::get_end_point() const
	//{
	//	return this->end_point;
	//}
	Direction Polygon::get_direction() const
	{
		return this->direction;
	}
	void Polygon::set_direction(Direction direction)
	{
		this->direction = direction;
	}
	void Polygon::ReverseDirection()
	{
		if (direction == Direction::Forward)
		{
			direction = Direction::Reverse;
		}
		else if (direction == Direction::Reverse)
		{
			direction = Direction::Forward;
		}
		else
		{
			std::cout << "Warning::Polygon::ReverseDirection::illegal direction" << std::endl;
		}
	}
}