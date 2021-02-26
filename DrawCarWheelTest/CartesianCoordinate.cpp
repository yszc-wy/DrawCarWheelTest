#include "CartesianCoordinate.h"

#include <iostream>

#include "Range.h"
namespace Coordinate {
	const float CartesianCoordinate::kAxisTextDisToAxisEnd = 9.0f;
	const float CartesianCoordinate::kXAxisMarkTextDisToMarkEnd = 4.5f;
	const float CartesianCoordinate::kYAxisMarkTextDisToMarkEnd = 4.5f;
	const float CartesianCoordinate::kZAxisMarkTextDisToAxis = 10.0f;
	const float CartesianCoordinate::kTextMarkExtend =10.0f;
	const float CartesianCoordinate::kAxisExtend = 20.0f;
	const float CartesianCoordinate::kNoTextMarkExtend = 5.0f;
	const float CartesianCoordinate::kArrowWidth = 10.0f;
	const float CartesianCoordinate::kArrowLength = kAxisExtend / 2.0f;

	void CartesianCoordinate::set_axis_extend(float axis_extend)
	{
		this->axis_extend = axis_extend;
		this->arrow_length = axis_extend / 2.0f;
	}
	void CartesianCoordinate::set_axis_text_dis_to_axis_end(float axis_text_dis_to_axis_end)
	{
		this->axis_text_dis_to_axis_end = axis_text_dis_to_axis_end;
	}
	void CartesianCoordinate::set_arrow_width(float arrow_width)
	{
		this->arrow_width = arrow_width;
	}
	//axis length
	void CartesianCoordinate::set_axis_length(Axis axis, float length)
	{
		if (length <= 0)
		{
			std::cout << "Error::CartesianCoordinate::set_axis_length::illiage length" << std::endl;
			return;
		}
		switch (axis)
		{
		case Axis::X:
			this->axis_length.x = length;
			break;
		case Axis::Y:
			this->axis_length.y = length;
			break;
		case Axis::Z:
			this->axis_length.z = length;
			break;
		default:
			std::cout << "Error::CartesianCoordinate::set_axis_length::illiage Axis id" << std::endl;
			break;
		}
	}
	void CartesianCoordinate::set_axis_length(float x_length, float y_length, float z_length)
	{
		if (x_length <= 0|| y_length <= 0||z_length<=0)
		{
			std::cout << "Error::CartesianCoordinate::set_axis_length::illiage length" << std::endl;
			return;
		}
		this->axis_length.x = x_length;
		this->axis_length.y = y_length;
		this->axis_length.z = z_length;
	}

	//axis text
	void CartesianCoordinate::set_axis_text(Axis axis, const Text& text)
	{
		switch (axis)
		{
		case Axis::X:
			this->x_axis_text = text;
			break;
		case Axis::Y:
			this->y_axis_text = text;
			break;
		case Axis::Z:
			this->z_axis_text = text;
			break;
		default:
			std::cout << "Error::CartesianCoordinate::set_axis_text::illiage Axis id" << std::endl;
			break;
		}
	}
	void CartesianCoordinate::set_axis_text(const Text& x_text, const Text& y_text, const Text& z_text)
	{
		this->x_axis_text = x_text;
		this->y_axis_text = y_text;
		this->z_axis_text = z_text;
	}
	//axis color
	void CartesianCoordinate::set_axis_color(Axis axis, const glm::vec3& color)
	{
		switch (axis)
		{
		case Axis::X:
			this->x_axis_color = color;
			this->x_axis_text.set_color(color);
			break;
		case Axis::Y:
			this->y_axis_color = color;
			this->y_axis_text.set_color(color);
			break;
		case Axis::Z:
			this->z_axis_color = color;
			this->z_axis_text.set_color(color);
			break;
		default:
			std::cout << "Error::CartesianCoordinate::set_axis_color::illiage Axis id" << std::endl;
			break;
		}
	}
	void CartesianCoordinate::set_axis_color(const glm::vec3& x_color, const glm::vec3& y_color, const glm::vec3& z_color)
	{
		this->x_axis_color = x_color;
		//this->x_axis_text.set_color(x_color);
		this->y_axis_color = y_color;
		//this->y_axis_text.set_color(y_color);
		this->z_axis_color = z_color;
		//this->z_axis_text.set_color(z_color);
	}
	//coordinate quadrant
	void CartesianCoordinate::set_coordinate_octant(unsigned char coordinate_octant)
	{
		if (coordinate_octant == 0)
		{
			std::cout << "Error::CartesianCoordinate::set_coordinate_octant" << std::endl;
			return;
		}
		this->coordinate_octant = coordinate_octant;
	}
	//add mark
	void CartesianCoordinate::AddMark(Axis axis, const Mark& mark)
	{
		if (mark.position_ratio < -1 || mark.position_ratio>1)
		{
			std::cout << "Error::CartesianCoordinate::AddMark::illegal position_ratio" << std::endl;
		}
		switch (axis)
		{
		case Axis::X:
			this->x_axis_marks.push_back(mark);
			break;
		case Axis::Y:
			this->y_axis_marks.push_back(mark);
			break;
		case Axis::Z:
			this->z_axis_marks.push_back(mark);
			break;
		default:
			std::cout << "Error::CartesianCoordinate::AddMark::illiage Axis id" << std::endl;
			break;
		}
	}
	bool CartesianCoordinate::Judge_Octant(int q_id)
	{
		if (q_id < 0 || q_id>8)
		{
			std::cout << "Error::CartesianCoordinate::Judge_Quadrant::illiage q_id" << std::endl;
			return false;
		}
		int move_count = q_id - 1;
		unsigned char q = 1;
		while (move_count--)
		{
			q = q << 1;
		}

		if (q&this->coordinate_octant != 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//load
	Object::CoordinateObject CartesianCoordinate::LoadObject(const Texture& font_texture)
	{
		//判断x，y，z轴的范围
		Range axis_range[3];
		this->GetAxisRange(axis_range);

		Range mark_range[8];
		this->GetMarkRange(mark_range);
		
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		//x轴
		if (axis_length.x <= 0.0f)
		{
			std::cout << "Error::CartesianCoordinate::LoadObject::illegal x_length"<<std::endl;
			return Object::CoordinateObject();
		}
		float x_start_extend=0;
		float x_end_extend=0;
		if (axis_range[0].GetStart() == -1)
		{
			x_start_extend = 0 - this->axis_extend;
		}

		if (axis_range[0].GetEnd() == 1)
		{
			x_end_extend = this->axis_extend;

			//添加三角箭头
			Vertex acme;
			acme.Position = glm::vec3(axis_range[0].GetEnd()*axis_length.x + x_end_extend, 0.0f, 0.0f);
			acme.Color = x_axis_color;

			Vertex lp;
			lp.Position = glm::vec3(axis_range[0].GetEnd()*axis_length.x+ x_end_extend- this->arrow_length, -this->arrow_width, 0.0f);
			lp.Color = x_axis_color;

			Vertex rp;
			rp.Position = glm::vec3(axis_range[0].GetEnd()*axis_length.x+x_end_extend- this->arrow_length, this->arrow_width, 0.0f);
			rp.Color = x_axis_color;

			int start_id = vertices.size();
			vertices.push_back(acme);
			vertices.push_back(lp);
			vertices.push_back(rp);

			indices.push_back(start_id);
			indices.push_back(start_id+1);

			indices.push_back(start_id+1);
			indices.push_back(start_id + 2);

			indices.push_back(start_id+2);
			indices.push_back(start_id);
		}

		
		unsigned int x_axis_start_point_id= vertices.size();
		Vertex x_axis_start_point;//0
		x_axis_start_point.Position = glm::vec3(axis_range[0].GetStart()*axis_length.x+ x_start_extend, 0.0f, 0.0f);
		x_axis_start_point.Color = this->x_axis_color;
		vertices.push_back(x_axis_start_point);

		Vertex x_axis_end_point;//1
		x_axis_end_point.Position = glm::vec3(axis_range[0].GetEnd()*axis_length.x+x_end_extend, 0.0f, 0.0f);
		x_axis_end_point.Color = this->x_axis_color;
		vertices.push_back(x_axis_end_point);

		indices.push_back(x_axis_start_point_id);
		indices.push_back(x_axis_start_point_id+1);

		//y轴
		if (axis_length.y <= 0.0f)
		{
			std::cout << "Error::CartesianCoordinate::LoadObject::illegal y_length" << std::endl;
			return Object::CoordinateObject();
		}

		float y_start_extend = 0;
		float y_end_extend = 0;
		if (axis_range[1].GetStart() == -1)
		{
			y_start_extend = 0 - this->axis_extend;
		}

		if (axis_range[1].GetEnd() == 1)
		{
			y_end_extend = this->axis_extend;

			//添加三角箭头
			Vertex acme;
			acme.Position = glm::vec3(0.0f, axis_range[1].GetEnd()*axis_length.y + y_end_extend, 0.0f);
			acme.Color = y_axis_color;

			Vertex lp;
			lp.Position = glm::vec3(-this->arrow_width, axis_range[1].GetEnd()*axis_length.y + y_end_extend- this->arrow_length, 0.0f);
			lp.Color = y_axis_color;

			Vertex rp;
			rp.Position = glm::vec3(this->arrow_width, axis_range[1].GetEnd()*axis_length.y + y_end_extend - this->arrow_length, 0.0f);
			rp.Color = y_axis_color;

			int start_id = vertices.size();
			vertices.push_back(acme);
			vertices.push_back(lp);
			vertices.push_back(rp);

			indices.push_back(start_id);
			indices.push_back(start_id + 1);

			indices.push_back(start_id + 1);
			indices.push_back(start_id + 2);

			indices.push_back(start_id + 2);
			indices.push_back(start_id);
		}
		
		unsigned int y_axis_start_point_id = vertices.size();
		Vertex y_axis_start_point;//0
		y_axis_start_point.Position = glm::vec3(0.0f, axis_range[1].GetStart()*axis_length.y + y_start_extend, 0.0f);
		y_axis_start_point.Color = this->y_axis_color;
		vertices.push_back(y_axis_start_point);

		Vertex y_axis_end_point;//1
		y_axis_end_point.Position = glm::vec3(0.0f, axis_range[1].GetEnd()*axis_length.y + y_end_extend, 0.0f);
		y_axis_end_point.Color = this->y_axis_color;
		vertices.push_back(y_axis_end_point);

		indices.push_back(y_axis_start_point_id);
		indices.push_back(y_axis_start_point_id + 1);
		//z轴
		if (axis_length.z <= 0.0f)
		{
			std::cout << "Error::CartesianCoordinate::LoadObject::illegal z_length" << std::endl;
			return Object::CoordinateObject();
		}

		float z_start_extend = 0;
		float z_end_extend = 0;
		if (axis_range[2].GetStart() == -1)
		{
			z_start_extend = 0 - this->axis_extend;
		}

		if (axis_range[2].GetEnd() == 1)
		{
			z_end_extend = this->axis_extend;
			//添加三角箭头
			Vertex acme;
			acme.Position = glm::vec3(0.0f, 0.0f, axis_range[2].GetEnd()*axis_length.z + z_end_extend);
			acme.Color = z_axis_color;

			Vertex lp;
			lp.Position = glm::vec3(0.0f, -this->arrow_width, axis_range[2].GetEnd()*axis_length.z + z_end_extend- this->arrow_length);
			lp.Color = z_axis_color;

			Vertex rp;
			rp.Position = glm::vec3(0.0f, this->arrow_width, axis_range[2].GetEnd()*axis_length.z + z_end_extend - this->arrow_length);
			rp.Color = z_axis_color;

			int start_id = vertices.size();
			vertices.push_back(acme);
			vertices.push_back(lp);
			vertices.push_back(rp);

			indices.push_back(start_id);
			indices.push_back(start_id + 1);

			indices.push_back(start_id + 1);
			indices.push_back(start_id + 2);

			indices.push_back(start_id + 2);
			indices.push_back(start_id);
		}

		unsigned int z_axis_start_point_id = vertices.size();
		Vertex z_axis_start_point;//0
		z_axis_start_point.Position = glm::vec3(0.0f, 0.0f, axis_range[2].GetStart()*axis_length.z + z_start_extend);
		z_axis_start_point.Color = this->z_axis_color;
		vertices.push_back(z_axis_start_point);

		Vertex z_axis_end_point;//1
		z_axis_end_point.Position = glm::vec3(0.0f, 0.0f, axis_range[2].GetEnd()*axis_length.z + z_end_extend);
		z_axis_end_point.Color = this->z_axis_color;
		vertices.push_back(z_axis_end_point);

		indices.push_back(z_axis_start_point_id);
		indices.push_back(z_axis_start_point_id + 1);

		//mark line
		//x line mark
		for (auto &mark : this->x_axis_marks)
		{
			//判断该mark在不在轴上
			if (mark.position_ratio<axis_range[0].GetMin() || mark.position_ratio>axis_range[0].GetMax())
			{
				continue;
			}
			if (mark.position_ratio == 0)//不画线
			{
				continue;
			}

			Range range;
			if (mark.position_ratio > 0)
			{
				range = mark_range[MarkType::X_PLUS_MARK];
				
			}
			else if (mark.position_ratio < 0)
			{
				range = mark_range[MarkType::X_MINUS_MARK];
			}

			if (range.GetStart() == range.GetEnd())//该范围内不存在mark
			{
				continue;
			}

			float extend=0;
			if (mark.text.get_text() == "")
			{
				extend = kNoTextMarkExtend;
			}
			else
			{
				extend = kTextMarkExtend;
			}

			float mark_start_extend = 0;
			float mark_end_extend = 0;
			if (range.GetStart() == -1)
			{
				mark_start_extend = 0 - extend;
			}
			if (range.GetEnd() == 1)
			{
				mark_end_extend = extend;
			}

			Vertex start_point;
			start_point.Position = glm::vec3(0.0f + this->axis_length.x*mark.position_ratio, range.GetStart()*this->axis_length.y+ mark_start_extend, 0.0f);
			start_point.Color = mark.line_color;

			Vertex end_point;
			end_point.Position = glm::vec3(0.0f + this->axis_length.x*mark.position_ratio, range.GetEnd()*this->axis_length.y+mark_end_extend, 0.0f);
			end_point.Color = mark.line_color;

			unsigned start_point_id = vertices.size();
			vertices.push_back(start_point);
			vertices.push_back(end_point);

			indices.push_back(start_point_id);//mark
			indices.push_back(start_point_id+1);
			
		}
		//y line mark
		for (auto &mark : this->y_axis_marks)
		{
			//判断该mark在不在轴上
			if (mark.position_ratio<axis_range[1].GetMin() || mark.position_ratio>axis_range[1].GetMax())
			{
				continue;
			}
			if (mark.position_ratio == 0)//不画线
			{
				continue;
			}

			Range range;
			if (mark.position_ratio > 0)
			{
				range = mark_range[MarkType::Y_PLUS_MARK];

			}
			else if (mark.position_ratio < 0)
			{
				range = mark_range[MarkType::Y_MINUS_MARK];
			}
			if (range.GetStart() == range.GetEnd())//该范围内不存在mark
			{
				continue;
			}

			float extend = 0;
			if (mark.text.get_text() == "")
			{
				extend = kNoTextMarkExtend;
			}
			else
			{
				extend = kTextMarkExtend;
			}

			float mark_start_extend = 0;
			float mark_end_extend = 0;
			if (range.GetStart() == -1)
			{
				mark_start_extend = 0 - extend;
			}
			if (range.GetEnd() == 1)
			{
				mark_end_extend = extend;
			}

			Vertex start_point;
			start_point.Position = glm::vec3(range.GetStart()*this->axis_length.x+ mark_start_extend, 0.0f+this->axis_length.y*mark.position_ratio, 0.0f);
			start_point.Color = mark.line_color;

			Vertex end_point;
			end_point.Position = glm::vec3(range.GetEnd()*this->axis_length.x+ mark_end_extend, 0.0f + this->axis_length.y*mark.position_ratio, 0.0f);
			end_point.Color = mark.line_color;

			unsigned start_point_id = vertices.size();
			vertices.push_back(start_point);
			vertices.push_back(end_point);

			indices.push_back(start_point_id);//mark
			indices.push_back(start_point_id + 1);

		}

		//z line mark
		for (auto &mark : this->z_axis_marks)
		{
			//判断该mark在不在轴上
			if (mark.position_ratio<axis_range[2].GetMin() || mark.position_ratio>axis_range[2].GetMax())
			{
				continue;
			}
			if (mark.position_ratio == 0)//不画线
			{
				continue;
			}

			Range zox_range;
			Range zoy_range;
			if (mark.position_ratio > 0)
			{
				zox_range = mark_range[MarkType::ZOX_PLUS_MARK];
				zoy_range = mark_range[MarkType::ZOY_PLUS_MARK];
			}
			else if (mark.position_ratio < 0)
			{
				zox_range = mark_range[MarkType::ZOX_MINUS_MARK];
				zoy_range = mark_range[MarkType::ZOY_MINUS_MARK];
			}

			if (zox_range.GetStart() != zox_range.GetEnd())//该范围内不存在mark
			{
				Vertex start_point;
				start_point.Position = glm::vec3(zox_range.GetStart()*this->axis_length.x, 0.0f, 0.0f + this->axis_length.z*mark.position_ratio);
				start_point.Color = mark.line_color;

				Vertex end_point;
				end_point.Position = glm::vec3(zox_range.GetEnd()*this->axis_length.x, 0.0f, 0.0f + this->axis_length.z*mark.position_ratio);
				end_point.Color = mark.line_color;

				unsigned start_point_id = vertices.size();
				vertices.push_back(start_point);
				vertices.push_back(end_point);

				indices.push_back(start_point_id);//mark
				indices.push_back(start_point_id + 1);
			}
			if (zoy_range.GetStart() != zoy_range.GetEnd())
			{
				Vertex start_point;
				start_point.Position = glm::vec3(0.0f, zoy_range.GetStart()*this->axis_length.y, 0.0f + this->axis_length.z*mark.position_ratio);
				start_point.Color = mark.line_color;
				

				Vertex end_point;
				end_point.Position = glm::vec3(0.0f, zoy_range.GetEnd()*this->axis_length.y, 0.0f + this->axis_length.z*mark.position_ratio);
				end_point.Color = mark.line_color;

				unsigned start_point_id = vertices.size();
				vertices.push_back(start_point);
				vertices.push_back(end_point);

				indices.push_back(start_point_id);//mark
				indices.push_back(start_point_id + 1);
			}
		}

		Object::LineObject coords_line(OpenglObj::Line(vertices, indices), glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		//mark text

		std::vector<Object::MeshObject> mark_texts;
		std::vector<glm::vec3> mark_text_origin_positions;

		//添加轴text
		//x axis
		if (axis_range[0].GetMin() == -1)//添加负向文本
		{
			glm::vec3 text_position = glm::vec3(0.0f + axis_length.x*axis_range[0].GetMin()- this->axis_extend - this->axis_text_dis_to_axis_end,0.0f, 0.0f);
			OpenglObj::OpenglPointMsg font_point_msg = Font::LoadFontPoint(this->x_axis_text.get_text(), text_position, this->x_axis_text.get_size());
			Object::MeshObject text_mesh(OpenglObj::Mesh(font_point_msg, std::vector<Texture>{font_texture}), glm::mat4(1.0f), this->x_axis_text.get_color());
			mark_texts.push_back(text_mesh);
			mark_text_origin_positions.push_back(text_position);
		}
		if (axis_range[0].GetMax() == 1)//添加正向文本
		{
			glm::vec3 text_position = glm::vec3(0.0f + axis_length.x*axis_range[0].GetMax()+ this->axis_extend + this->axis_text_dis_to_axis_end, 0.0f, 0.0f);
			OpenglObj::OpenglPointMsg font_point_msg = Font::LoadFontPoint(this->x_axis_text.get_text(), text_position, this->x_axis_text.get_size());
			Object::MeshObject text_mesh(OpenglObj::Mesh(font_point_msg, std::vector<Texture>{font_texture}), glm::mat4(1.0f), this->x_axis_text.get_color());
			mark_texts.push_back(text_mesh);
			mark_text_origin_positions.push_back(text_position);
		}
		//y axis
		if (axis_range[1].GetMin() == -1)//添加负向文本
		{
			glm::vec3 text_position = glm::vec3(0.0f, axis_length.y*axis_range[1].GetMin() - this->axis_extend - this->axis_text_dis_to_axis_end, 0.0f);
			OpenglObj::OpenglPointMsg font_point_msg = Font::LoadFontPoint(this->y_axis_text.get_text(), text_position, this->y_axis_text.get_size());
			Object::MeshObject text_mesh(OpenglObj::Mesh(font_point_msg, std::vector<Texture>{font_texture}), glm::mat4(1.0f), this->y_axis_text.get_color());
			mark_texts.push_back(text_mesh);
			mark_text_origin_positions.push_back(text_position);
		}
		if (axis_range[1].GetMax() == 1)//添加正向文本
		{
			glm::vec3 text_position = glm::vec3(0.0f, axis_length.y*axis_range[1].GetMax() + this->axis_extend + this->axis_text_dis_to_axis_end, 0.0f);
			OpenglObj::OpenglPointMsg font_point_msg = Font::LoadFontPoint(this->y_axis_text.get_text(), text_position, this->y_axis_text.get_size());
			Object::MeshObject text_mesh(OpenglObj::Mesh(font_point_msg, std::vector<Texture>{font_texture}), glm::mat4(1.0f), this->y_axis_text.get_color());
			mark_texts.push_back(text_mesh);
			mark_text_origin_positions.push_back(text_position);
		}
		//z axis
		if (axis_range[2].GetMin() == -1)//添加负向文本
		{
			//0.0f + this->axis_length.z*mark.position_ratio
			glm::vec3 text_position = glm::vec3(0.0f,0.0f, axis_length.z*axis_range[2].GetMin() - this->axis_extend - this->axis_text_dis_to_axis_end);
			OpenglObj::OpenglPointMsg font_point_msg = Font::LoadFontPoint(this->z_axis_text.get_text(), text_position, this->z_axis_text.get_size());
			Object::MeshObject text_mesh(OpenglObj::Mesh(font_point_msg, std::vector<Texture>{font_texture}), glm::mat4(1.0f), this->z_axis_text.get_color());
			mark_texts.push_back(text_mesh);
			mark_text_origin_positions.push_back(text_position);
		}
		if (axis_range[2].GetMax() == 1)//添加正向文本
		{
			glm::vec3 text_position = glm::vec3(0.0f, 0.0f, axis_length.z*axis_range[2].GetMax() + this->axis_extend + this->axis_text_dis_to_axis_end);
			OpenglObj::OpenglPointMsg font_point_msg = Font::LoadFontPoint(this->z_axis_text.get_text(), text_position, this->z_axis_text.get_size());
			Object::MeshObject text_mesh(OpenglObj::Mesh(font_point_msg, std::vector<Texture>{font_texture}), glm::mat4(1.0f), this->z_axis_text.get_color());
			mark_texts.push_back(text_mesh);
			mark_text_origin_positions.push_back(text_position);
		}

		//添加mark text
		//x 轴
		for (auto &mark : this->x_axis_marks)
		{
			//判断该mark在不在轴上
			if (mark.position_ratio<axis_range[0].GetMin() || mark.position_ratio>axis_range[0].GetMax())
			{
				continue;
			}

			std::string text = mark.text.get_text();
			if (text == "")
			{
				continue;
			}

			Range range;
			if (mark.position_ratio > 0)
			{
				range = mark_range[MarkType::X_PLUS_MARK];

			}
			else if (mark.position_ratio < 0)
			{
				range = mark_range[MarkType::X_MINUS_MARK];
			}
			if (range.GetStart() == range.GetEnd())//该范围内不存在mark
			{
				continue;
			}

			if (range.GetMin() == -1)
			{
				glm::vec3 text_position = glm::vec3(0.0f + this->axis_length.x*mark.position_ratio, range.GetMin()*this->axis_length.y-kTextMarkExtend-kXAxisMarkTextDisToMarkEnd, 0.0f);
				OpenglObj::OpenglPointMsg font_point_msg = Font::LoadFontPoint(text, text_position,mark.text.get_size());
				Object::MeshObject text_mesh(OpenglObj::Mesh(font_point_msg, std::vector<Texture>{font_texture}), glm::mat4(1.0f), mark.text.get_color());
				mark_texts.push_back(text_mesh);
				mark_text_origin_positions.push_back(text_position);
			}
			if (range.GetMax() == 1)
			{
				glm::vec3 text_position = glm::vec3(0.0f + this->axis_length.x*mark.position_ratio, range.GetMax()*this->axis_length.y + kTextMarkExtend + kXAxisMarkTextDisToMarkEnd, 0.0f);
				OpenglObj::OpenglPointMsg font_point_msg = Font::LoadFontPoint(text, text_position, mark.text.get_size());
				Object::MeshObject text_mesh(OpenglObj::Mesh(font_point_msg, std::vector<Texture>{font_texture}), glm::mat4(1.0f), mark.text.get_color());
				mark_texts.push_back(text_mesh);
				mark_text_origin_positions.push_back(text_position);
			}
		}

		//y 轴
		for (auto &mark : this->y_axis_marks)
		{
			//判断该mark在不在轴上
			if (mark.position_ratio<axis_range[1].GetMin() || mark.position_ratio>axis_range[1].GetMax())
			{
				continue;
			}

			std::string text = mark.text.get_text();
			if (text == "")
			{
				continue;
			}

			Range range;
			if (mark.position_ratio > 0)
			{
				range = mark_range[MarkType::Y_PLUS_MARK];

			}
			else if (mark.position_ratio < 0)
			{
				range = mark_range[MarkType::Y_MINUS_MARK];
			}
			if (range.GetStart() == range.GetEnd())//该范围内不存在mark
			{
				continue;
			}

			if (range.GetMin() == -1)
			{
				glm::vec3 text_position = glm::vec3(range.GetMin()*this->axis_length.x - kTextMarkExtend - kYAxisMarkTextDisToMarkEnd, 0.0f + this->axis_length.y*mark.position_ratio, 0.0f);
				OpenglObj::OpenglPointMsg font_point_msg = Font::LoadFontPoint(text, text_position, mark.text.get_size());
				Object::MeshObject text_mesh(OpenglObj::Mesh(font_point_msg, std::vector<Texture>{font_texture}), glm::mat4(1.0f), mark.text.get_color());
				mark_texts.push_back(text_mesh);
				mark_text_origin_positions.push_back(text_position);
			}
			if (range.GetMax() == 1)
			{
				glm::vec3 text_position = glm::vec3(range.GetMax()*this->axis_length.x + kTextMarkExtend + kYAxisMarkTextDisToMarkEnd, 0.0f + this->axis_length.y*mark.position_ratio, 0.0f);
				OpenglObj::OpenglPointMsg font_point_msg = Font::LoadFontPoint(text, text_position, mark.text.get_size());
				Object::MeshObject text_mesh(OpenglObj::Mesh(font_point_msg, std::vector<Texture>{font_texture}), glm::mat4(1.0f), mark.text.get_color());
				mark_texts.push_back(text_mesh);
				mark_text_origin_positions.push_back(text_position);
			}
		}

		//z轴
		for (auto &mark : this->z_axis_marks)
		{
			//判断该mark在不在轴上
			if (mark.position_ratio<axis_range[2].GetMin() || mark.position_ratio>axis_range[2].GetMax())
			{
				continue;
			}

			std::string text = mark.text.get_text();
			if (text == "")
			{
				continue;
			}
	
			glm::vec3 text_position = glm::vec3(0.0f-kZAxisMarkTextDisToAxis/sqrt(2), 0.0f - kZAxisMarkTextDisToAxis / sqrt(2), this->axis_length.z*mark.position_ratio);
			OpenglObj::OpenglPointMsg font_point_msg = Font::LoadFontPoint(text, text_position, mark.text.get_size());
			Object::MeshObject text_mesh(OpenglObj::Mesh(font_point_msg, std::vector<Texture>{font_texture}), glm::mat4(1.0f), mark.text.get_color());
			mark_texts.push_back(text_mesh);
			mark_text_origin_positions.push_back(text_position);
		}

		return Object::CoordinateObject(coords_line, mark_texts, mark_text_origin_positions);
	}

	void CartesianCoordinate::GetAxisRange(Range *range)//3个range
	{
		if (this->coordinate_octant == 0)
		{
			std::cout << "Error::CartesianCoordinate::LoadObject::illage coordinate_octant" << std::endl;
			range[0] = Range();
			range[1] = Range();
			range[2] = Range();
			return;
		}

		unsigned char x_plus = 0x99;
		unsigned char x_minus = ~x_plus;
		unsigned char x_plus_result = x_plus&this->coordinate_octant;
		unsigned char x_minus_result = x_minus&this->coordinate_octant;
		Range x_range;
		if (x_plus_result != 0 && x_minus_result != 0)
		{
			x_range.SetRange(-1, 1);
		}
		else if (x_plus_result != 0 && x_minus_result == 0)
		{
			x_range.SetRange(0, 1);
		}
		else if (x_plus_result == 0 && x_minus_result != 0)
		{
			x_range.SetRange(-1, 0);
		}

		unsigned char y_plus = 0x33;
		unsigned char y_minus = ~y_plus;
		unsigned char y_plus_result = y_plus&this->coordinate_octant;
		unsigned char y_minus_result = y_minus&this->coordinate_octant;
		Range y_range;
		if (y_plus_result != 0 && y_minus_result != 0)
		{
			y_range.SetRange(-1, 1);
		}
		else if (y_plus_result != 0 && y_minus_result == 0)
		{
			y_range.SetRange(0, 1);
		}
		else if (y_plus_result == 0 && y_minus_result != 0)
		{
			y_range.SetRange(-1, 0);
		}

		unsigned char z_plus = 0x0f;
		unsigned char z_minus = ~z_plus;
		unsigned char z_plus_result = z_plus&this->coordinate_octant;
		unsigned char z_minus_result = z_minus&this->coordinate_octant;
		Range z_range;
		if (z_plus_result != 0 && z_minus_result != 0)
		{
			z_range.SetRange(-1, 1);
		}
		else if (z_plus_result != 0 && z_minus_result == 0)
		{
			z_range.SetRange(0, 1);
		}
		else if (z_plus_result == 0 && z_minus_result != 0)
		{
			z_range.SetRange(-1, 0);
		}

		range[0] = x_range;
		range[1] = y_range;
		range[2] = z_range;
	}
	void CartesianCoordinate::GetMarkRange(Range *range)//8个range
	{
		unsigned char xoy_mark = 0x0f & ((this->coordinate_octant >> 4) | (this->coordinate_octant));//从低到高是xoy平面的第一到第四象限

		unsigned char yoz_back = (0x04 & (this->coordinate_octant >> 4)) |(0x08 & (this->coordinate_octant >> 2)) | (0x03 & (this->coordinate_octant >> 1));//交换后两位调节象限顺序
		unsigned char yoz_front = (0x08 & (this->coordinate_octant >> 1)) | (0x04 & (this->coordinate_octant >> 5)) | (0x02 & (this->coordinate_octant >> 2)) | (0x01 & this->coordinate_octant);//同样交换
		unsigned char yoz_mark = yoz_front|yoz_back;//从低到高是yoz平面的第一到第四象限

		unsigned char xoz_front = (0x08 & (this->coordinate_octant >> 1)) | (0x04 & (this->coordinate_octant >> 3)) | (0x03 & this->coordinate_octant);
		unsigned char xoz_back = (0x0c & (this->coordinate_octant >> 4)) | (0x02 & (this->coordinate_octant >> 1)) | (0x01 & (this->coordinate_octant >> 3));
		unsigned char xoz_mark = xoz_front|yoz_back;//从低到高是xoz平面的第一到第四象限

		unsigned char x_plus_mark = 0x09&xoy_mark;//x正轴的mark范围由第一和第四象限决定
		Range x_plus_mark_range;
		switch (x_plus_mark)
		{
		case 0x01:
			x_plus_mark_range.SetRange(0, 1);
			break;
		case 0x08:
			x_plus_mark_range.SetRange(-1, 0);
			break;
		case 0x09:
			x_plus_mark_range.SetRange(-1, 1);
			break;
		default:
			x_plus_mark_range.SetRange(0, 0);
			break;
		}

		unsigned char x_minus_mark = 0x06 & xoy_mark;
		Range x_minus_mark_range;
		switch (x_minus_mark)
		{
		case 0x02:
			x_minus_mark_range.SetRange(0, 1);
			break;
		case 0x04:
			x_minus_mark_range.SetRange(-1, 0);
			break;
		case 0x06:
			x_minus_mark_range.SetRange(-1, 1);
			break;
		default:
			x_minus_mark_range.SetRange(0, 0);
			break;
		}

		unsigned char y_plus_mark = 0x03 & xoy_mark;
		Range y_plus_mark_range;
		switch (y_plus_mark)
		{
		case 0x01:
			y_plus_mark_range.SetRange(0, 1);
			break;
		case 0x02:
			y_plus_mark_range.SetRange(-1, 0);
			break;
		case 0x03:
			y_plus_mark_range.SetRange(-1, 1);
			break;
		default:
			y_plus_mark_range.SetRange(0, 0);
			break;
		}

		unsigned char y_minus_mark = 0x0c & xoy_mark;
		Range y_minus_mark_range;
		switch (y_minus_mark)
		{
		case 0x08:
			y_minus_mark_range.SetRange(0, 1);
			break;
		case 0x04:
			y_minus_mark_range.SetRange(-1, 0);
			break;
		case 0x0c:
			y_minus_mark_range.SetRange(-1, 1);
			break;
		default:
			y_minus_mark_range.SetRange(0, 0);
			break;
		}

		unsigned char zox_plus_mark = 0x03 & xoz_mark;
		Range zox_plus_mark_range;
		switch (zox_plus_mark)
		{
		case 0x01:
			zox_plus_mark_range.SetRange(0, 1);
			break;
		case 0x02:
			y_minus_mark_range.SetRange(-1, 0);
			break;
		case 0x03:
			y_minus_mark_range.SetRange(-1, 1);
			break;
		default:
			y_minus_mark_range.SetRange(0, 0);
			break;
		}

		unsigned char zox_minus_mark = 0x0c & xoz_mark;
		Range zox_minus_mark_range;
		switch (zox_minus_mark)
		{
		case 0x08:
			zox_minus_mark_range.SetRange(0, 1);
			break;
		case 0x04:
			zox_minus_mark_range.SetRange(-1, 0);
			break;
		case 0x0c:
			zox_minus_mark_range.SetRange(-1, 1);
			break;
		default:
			zox_minus_mark_range.SetRange(0, 0);
			break;
		}

		unsigned char zoy_plus_mark = 0x03 & yoz_mark;
		Range zoy_plus_mark_range;
		switch (zoy_plus_mark)
		{
		case 0x01:
			zoy_plus_mark_range.SetRange(0, 1);
			break;
		case 0x02:
			zoy_plus_mark_range.SetRange(-1, 0);
			break;
		case 0x03:
			zoy_plus_mark_range.SetRange(-1, 1);
			break;
		default:
			zoy_plus_mark_range.SetRange(0, 0);
			break;
		}

		unsigned char zoy_minus_mark = 0x03 & yoz_mark;
		Range zoy_minus_mark_range;
		switch (zoy_minus_mark)
		{
		case 0x08:
			zoy_minus_mark_range.SetRange(0, 1);
			break;
		case 0x04:
			zoy_minus_mark_range.SetRange(-1, 0);
			break;
		case 0x0c:
			zoy_minus_mark_range.SetRange(-1, 1);
			break;
		default:
			zoy_minus_mark_range.SetRange(0, 0);
			break;
		}

		range[0] = x_plus_mark_range;
		range[1] = x_minus_mark_range;
		range[2] = y_plus_mark_range;
		range[3] = y_minus_mark_range;
		range[4] = zox_plus_mark_range;
		range[5] = zox_minus_mark_range;
		range[6] = zoy_plus_mark_range;
		range[7] = zoy_minus_mark_range;
	}
	
}