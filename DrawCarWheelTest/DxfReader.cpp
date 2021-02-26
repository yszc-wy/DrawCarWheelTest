#include "DxfReader.h"

#include <iostream>
#include <cmath>

#include "DxfLine.h"
#include "DxfCircle.h"

#include <iostream>
namespace DxfReader {
	const double DxfReader::kMinPointDis = 0.5f;

	DxfReader::DxfReader(): polygons(),sort_line(false), error_log(""){}
	void DxfReader::addLayer(const DL_LayerData& data) {}
	void DxfReader::addPoint(const DL_PointData& data) {}


	void DxfReader::addLine(const DL_LineData& data) {
		//printf("LINE     端点1:(%6.3f, %6.3f, %6.3f) 端点2:(%6.3f, %6.3f, %6.3f) ",
		//	data.x1, data.y1, data.z1, data.x2, data.y2, data.z2);
		//Polygon(std::make_shared<Line>(data.x1, data.y1, data.x2, data.y2), Direction::Forward);
		//polygon.LoadSEPoint();
		polygons.push_back(Polygon(std::make_shared<Line>(data.x1, data.y1, data.x2, data.y2), Direction::Forward));
		//std::cout << "polygons size:" << polygons.size() << std::endl;
	}

	void DxfReader::addArc(const DL_ArcData& data) {
		//printf("ARC      圆心:(%6.3f, %6.3f, %6.3f) 半径:%6.3f, 角度1:%6.3f, 角度2:%6.3f ",
		//	data.cx, data.cy, data.cz,
		//	data.radius, data.angle1, data.angle2);
		//Polygon(std::make_shared<Circle>(data.cx, data.cy, data.radius, data.angle1, data.angle2), Direction::Forward);
		//polygon.LoadSEPoint();
		polygons.push_back(Polygon(std::make_shared<Circle>(data.cx, data.cy, data.radius, data.angle1, data.angle2), Direction::Forward));
		//std::cout << "polygons size:" << polygons.size() << std::endl;
	}

	void DxfReader::addCircle(const DL_CircleData& data) {}

	void DxfReader::addPolyline(const DL_PolylineData& data) {}

	void DxfReader::addVertex(const DL_VertexData& data) {}

	void DxfReader::add3dFace(const DL_3dFaceData& data) {}

	void DxfReader::SortToLine()
	{
		auto polygons_size = this->polygons.size();
		if (this->polygons.size() == 0)
		{
			std::cout << "Warning::DxfReader::SortToLine::No value in polygons" << std::endl;
			this->error_log += "Warning::There is no Line and Arc polygons in this file\n";
			return;
		}
		//std::cout << "Origin msg" << std::endl;
		//for (int i=0;i!=this->polygons.size();++i)
		//{
		//	this->polygons[i].get_p_ipolygon()->ShowType();
		//	std::cout << "Direction:" << this->polygons[i].get_direction() << " start point: x:" << this->polygons[i].get_p_ipolygon()->GetStartPoint().x << " y:" << this->polygons[i].get_p_ipolygon()->GetStartPoint().y
		//		<< " end point: x:" << this->polygons[i].get_p_ipolygon()->GetEndPoint().x << " y:" << this->polygons[i].get_p_ipolygon()->GetEndPoint().y << std::endl;
		//}
		//std::cout << std::endl;

		//auto begin = this->polygons.begin();
		//this->polygons.erase(begin);

		//std::cout << "Re Origin msg" << std::endl;
		//for (int i = 0; i != this->polygons.size(); ++i)
		//{
		//	this->polygons[i].get_p_ipolygon()->ShowType();
		//	std::cout << "Direction:" << this->polygons[i].get_direction() << " start point: x:" << this->polygons[i].get_start_point().x << " y:" << this->polygons[i].get_start_point().y
		//		<< " end point: x:" << this->polygons[i].get_end_point().x << " y:" << this->polygons[i].get_end_point().y << std::endl;
		//}
		//std::cout << std::endl;


		
		std::list<Polygon> polygon_line_list;
		bool left_have_connect = true;
		bool right_have_connect = true;
		while(!(left_have_connect==false&& right_have_connect==false))
		{
			if (polygon_line_list.size() == 0)
			{
				//std::cout << "first: ";
				polygon_line_list.push_back(this->polygons[0]);
				//this->polygons[0].Show();
				//this->polygons[1].Show();
				//this->polygons[2].Show();
				//std::cout << "Size1:" << this->polygons.size() << std::endl;
				this->polygons.erase(this->polygons.begin());


				//std::cout << "Size2:" << this->polygons.size() << std::endl;
				//this->polygons[0].Show();
				//this->polygons[1].Show();
				//this->polygons[2].Show();
				//this->polygons.begin()->Show();
				//std::cout << "ok:?" << std::endl;
			}
			else
			{
				//左边连接
				Point left_point;
				if (polygon_line_list.begin()->get_direction() == Direction::Forward)
				{
					left_point = polygon_line_list.begin()->get_p_ipolygon()->GetStartPoint();
				}
				else if (polygon_line_list.begin()->get_direction() == Direction::Reverse)
				{
					left_point = polygon_line_list.begin()->get_p_ipolygon()->GetEndPoint();
				}
				//寻找可连接的polygon
				//Polygon left_connect;
				if (left_have_connect)
				{
					bool find_connect = false;
					for (int i = 0; i != this->polygons.size(); ++i)
					{
						//左边点符合条件,翻转
						if (GetPointsDis(this->polygons[i].get_p_ipolygon()->GetStartPoint(), left_point) <= kMinPointDis)
						{
							/*std::cout << "One:" << std::endl;
							std::cout << "getPoint:";
							this->polygons[i].get_start_point().Show();
							this->polygons[i].get_end_point().Show();

							std::cout << " leftPoint:";
							left_point.Show();
							std::cout << std::endl;*/

							this->polygons[i].set_direction(Direction::Reverse);
							polygon_line_list.push_front(this->polygons[i]);
							this->polygons.erase(this->polygons.begin() + i);
							find_connect = true;
							break;
						}

						//右边点符合条件
						if (GetPointsDis(this->polygons[i].get_p_ipolygon()->GetEndPoint(), left_point) <= kMinPointDis)
						{
							//std::cout << "One:" << std::endl;
							//std::cout << "getPoint:";
							//this->polygons[i].get_start_point().Show();
							//this->polygons[i].get_end_point().Show();
							//std::cout << " leftPoint:";
							//left_point.Show();
							//std::cout << std::endl;

							polygon_line_list.push_front(this->polygons[i]);
							this->polygons.erase(this->polygons.begin() + i);
							find_connect = true;
							break;
						}
					}
					if (!find_connect)
					{
						left_have_connect = false;
					}
				}

				//右边连接
				Point right_point;
				if (polygon_line_list.rbegin()->get_direction() == Direction::Forward)
				{
					right_point = polygon_line_list.rbegin()->get_p_ipolygon()->GetEndPoint();
				}
				else if (polygon_line_list.rbegin()->get_direction() == Direction::Reverse)
				{
					right_point = polygon_line_list.rbegin()->get_p_ipolygon()->GetStartPoint();
				}
				//寻找可连接的polygon
				if (right_have_connect)
				{
					bool find_connect = false;
					for (int i = 0; i != this->polygons.size(); ++i)
					{
						//左边点符合条件
						if (GetPointsDis(this->polygons[i].get_p_ipolygon()->GetStartPoint(), right_point) <= kMinPointDis)
						{
							polygon_line_list.push_back(this->polygons[i]);
							this->polygons.erase(this->polygons.begin() + i);
							find_connect = true;
							break;
						}
						
						//右边点符合条件,翻转
						if (GetPointsDis(this->polygons[i].get_p_ipolygon()->GetEndPoint(), right_point) <= kMinPointDis)
						{
							this->polygons[i].set_direction(Direction::Reverse);
							polygon_line_list.push_back(this->polygons[i]);
							this->polygons.erase(this->polygons.begin() + i);
							find_connect = true;
							break;
						}

					}
					if (!find_connect)
					{
						right_have_connect = false;
					}

				}

			}
		}
		if (this->polygons.size() != 0)
		{
			std::cout << "Warning::DxfReader::SortToLine::polygons.size!=0" << std::endl;
			this->error_log += "Can not group to a whole line,some of the line segments are left!\n";
			//this->polygons.clear();
			this->error_log += "	There are the left segments:\n";
			for (auto &i : polygons)
			{
				error_log += i.Get_Log();
			}
			this->polygons.clear();
		}

		//
		//<< "After msg" << std::endl;
		//std::cout << "Polygons:" << std::endl;
		//for (auto &i : polygons)
		//{
		//	i.get_p_ipolygon()->ShowType();
		//	std::cout << "Direction:" << i.get_direction() << " start point: x:" << i.get_p_ipolygon()->GetStartPoint().x << " y:" << i.get_p_ipolygon()->GetStartPoint().y
		//		<< " end point: x:" << i.get_p_ipolygon()->GetEndPoint().x << " y:" << i.get_p_ipolygon()->GetEndPoint().y << std::endl;
		//}
		//std::cout << std::endl;

		//std::cout << "List:" << std::endl;
		//for (auto &i : polygon_line_list)
		//{
		//	i.get_p_ipolygon()->ShowType();
		//	std::cout << "Direction:" << i.get_direction() << " start point: x:" << i.get_p_ipolygon()->GetStartPoint().x << " y:" << i.get_p_ipolygon()->GetStartPoint().y
		//		<< " end point: x:" << i.get_p_ipolygon()->GetEndPoint().x << " y:" << i.get_p_ipolygon()->GetEndPoint().y << std::endl;
		//	std::cout << std::endl;
		//}
		
		//this->polygons.clear();

		//x从小到大插入
		Point left_point;
		if (polygon_line_list.begin()->get_direction() == Direction::Forward)
		{
			left_point = polygon_line_list.begin()->get_p_ipolygon()->GetStartPoint();
		}
		else if (polygon_line_list.begin()->get_direction() == Direction::Reverse)
		{
			left_point = polygon_line_list.begin()->get_p_ipolygon()->GetEndPoint();
		}

		//std::cout << "Left OK?" << std::endl;
		Point right_point;
		if (polygon_line_list.rbegin()->get_direction() == Direction::Forward)
		{
			right_point = polygon_line_list.rbegin()->get_p_ipolygon()->GetEndPoint();
		}
		else if (polygon_line_list.rbegin()->get_direction() == Direction::Reverse)
		{
			right_point = polygon_line_list.rbegin()->get_p_ipolygon()->GetStartPoint();
		}

		if (left_point.x < right_point.x)
		{
			for (auto i = polygon_line_list.begin(); i != polygon_line_list.end(); ++i)
			{
				this->polygons.push_back(*i);
			}
			this->sort_line = true;
		}
		else if (left_point.x > right_point.x)
		{
			for (auto i = polygon_line_list.rbegin(); i != polygon_line_list.rend(); ++i)
			{
				i->ReverseDirection();
				this->polygons.push_back(*i);
			}
			this->sort_line = true;
		}
		else
		{
			std::cout << "Error:: DxfReader::SortToLine::left_point.x == right_point.x" << std::endl;
			this->error_log += "Error::There are some error when group to a whole line,may be polygons is illegal\n";
			//for (auto &i : polygon_line_list)
			//{
			//	i.get_p_ipolygon()->ShowType();
			//	std::cout << "Direction:" << i.get_direction() <<" start point: x:"<< i.get_start_point().x<<" y:"<< i.get_start_point().y 
			//		<< " end point: x:" << i.get_end_point().x << " y:" << i.get_end_point().y << std::endl;
			//	std::cout << std::endl;
			//}
		}
		//std::cout << "Left OK??" << std::endl;
		//std::cout << "Result:" << std::endl;
		//for (auto &i : polygons)
		//{
		//	i.get_p_ipolygon()->ShowType();
		//	std::cout << "Direction:" << i.get_direction() << " start point: x:" << i.get_start_point().x << " y:" << i.get_start_point().y
		//		<< " end point: x:" << i.get_end_point().x << " y:" << i.get_end_point().y << std::endl;
		//	std::cout << std::endl;
		//}

	}

	StandardFlangeTreadMsgMaker DxfReader::LoadSFTMsgMaker()
	{
		//std::cout << "LoadSFTMsgMaker:" << std::endl;
		if (!sort_line)
		{
			this->SortToLine();
		}
		//std::cout << "Load OK?" << std::endl;
		if (this->polygons.size() == 0)
		{
			std::cout << "Error::DxfReader::LoadSFTMsgMaker::empty polygons" << std::endl;
			return StandardFlangeTreadMsgMaker();
		}
		//std::cout << "Load OK??" << std::endl;
		return StandardFlangeTreadMsgMaker(this->polygons);
	}
	FloatPlantMsgMaker DxfReader::LoadFPMsgMaker()
	{
		//std::cout << "LoadFPMsgMaker:" << std::endl;
		if (!sort_line)
		{
			this->SortToLine();
		}

		if (this->polygons.size() == 0)
		{
			std::cout << "Error::DxfReader::LoadFPMsgMaker::empty polygons" << std::endl;
			return FloatPlantMsgMaker();
		}
		return FloatPlantMsgMaker(this->polygons);
	}

	double GetPointsDis(const Point& p1, const Point& p2)
	{
		return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
	}

	std::string DxfReader::get_error_log() const
	{
		return this->error_log;
	}
}