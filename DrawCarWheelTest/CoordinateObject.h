#pragma once
#include <cmath>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>


#include "LineObject.h"
#include "MeshObject.h"
#include "Font.h"
//聚合类，表示一些特定事物
namespace Object {
	class CoordinateObject
	{
	public:
		explicit CoordinateObject() :coordinate_line(), mark_texts(), mark_text_loc_positions(),model(glm::mat4(1.0f)) {}
		explicit CoordinateObject(const LineObject& coordinate_line, const std::vector<MeshObject>&mark_texts,const std::vector<glm::vec3>&mark_text_loc_positions)
			:coordinate_line(coordinate_line), 
			mark_texts(mark_texts),
			mark_text_loc_positions(mark_text_loc_positions),
			model(glm::mat4(1.0f))
		{
			if (this->mark_texts.size() != this->mark_text_loc_positions.size())
			{
				std::cout << "Error::CoordinateObject::CoordinateObject::illigel mark_texts size!=mark_text_origin_positions size";
			}
		}

		void UpdateTextDirection(const glm::vec3& camera_front,const glm::vec3& camera_up,const glm::mat4& projection,const glm::mat4& view)
		{
			auto text_size = this->mark_texts.size();

			//获取摄像机视线
			glm::vec3 text_target_front_wor = glm::vec3(0.0f, 0.0f, 0.0f) - camera_front;
			//std::cout << "coordinate:" << std::endl;
			for (int i = 0; i != text_size; ++i)
			{
				//std::cout << "coordinate_text:"<< i << std::endl;
				//glm::vec3 text_normal = glm::mat3(glm::transpose(glm::inverse(mark_texts[i].GetModel())))*glm::vec3(Font::kFontLocNormal[0], Font::kFontLocNormal[1], Font::kFontLocNormal[2]);
				//wor_position不受旋转的影响，只受整体model的影响
				glm::vec3 text_wor_position = glm::vec3(/*this->mark_texts[i].GetModel()*/this->model*glm::vec4(this->mark_text_loc_positions[i], 1.0f));
				
				//std::cout << "text_wor_position:" << "x:" << text_wor_position.x << " y:" << text_wor_position.y << " z:" << text_wor_position.z << std::endl;

				//glm::vec3 text_to_camera_wor = glm::normalize(camera_position - text_wor_position);
				glm::vec3 text_target_up_wor= glm::normalize(GetTextWorUp(text_target_front_wor, camera_up));

				//不能乘getModel，要用整体model，应为起点是原版方向不是实时调整
				//glm::vec3 text_to_camera_loc = glm::normalize(glm::inverse(glm::transpose(glm::inverse(glm::mat3(this->model))))*text_to_camera_wor);
				glm::vec3 text_target_front_loc= glm::normalize(glm::inverse(glm::transpose(glm::inverse(glm::mat3(this->model))))*text_target_front_wor); //
				glm::vec3 text_target_up_loc= glm::normalize(glm::inverse(glm::transpose(glm::inverse(glm::mat3(this->model))))*text_target_up_wor);

				glm::vec3 text_up_loc = glm::normalize(glm::vec3(Font::kFontLocUp[0], Font::kFontLocUp[1], Font::kFontLocUp[2]));
				glm::vec3 text_front_loc = glm::normalize(glm::vec3(Font::kFontLocFront[0], Font::kFontLocFront[1], Font::kFontLocFront[2]));

				//对front向量的旋转
				glm::vec3 rotate_axis_for_front= glm::normalize(glm::cross(text_front_loc, text_target_front_loc));
				float rotate_radian_for_front = glm::angle(text_front_loc, text_target_front_loc);
				glm::mat4 rotate_for_front = glm::rotate(glm::mat4(1.0f), rotate_radian_for_front, rotate_axis_for_front);//对齐front
				if (text_front_loc == text_target_front_loc || text_front_loc == -text_target_front_loc)
				{
					rotate_for_front = glm::mat4(1.0f);
				}

				glm::mat4 return_to_zero_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,0.0f)-this->mark_text_loc_positions[i]);
				glm::mat4 recover_translate= glm::translate(glm::mat4(1.0f), this->mark_text_loc_positions[i]);

				glm::mat4 text_model_for_front = recover_translate*rotate_for_front*return_to_zero_translate;

				glm::vec3 text_loc_up_after_for_front = glm::mat3(glm::transpose(glm::inverse(text_model_for_front)))*text_up_loc;

				//对up向量的旋转
				glm::vec3 rotate_axis_for_up = glm::normalize(glm::cross(text_loc_up_after_for_front, text_target_up_loc));
				float rotate_radian_for_up = glm::angle(text_loc_up_after_for_front, text_target_up_loc);
				glm::mat4 rotate_for_up = glm::rotate(glm::mat4(1.0f), rotate_radian_for_up, rotate_axis_for_up);//对齐front
				if (text_loc_up_after_for_front == text_target_up_loc || text_loc_up_after_for_front == -text_target_up_loc)
				{
					rotate_for_up = glm::mat4(1.0f);
				}

				glm::mat4 text_model_for_up= recover_translate*rotate_for_up*return_to_zero_translate;

				glm::mat4 text_model = this->model*text_model_for_up*text_model_for_front;

				this->mark_texts[i].SetModel(text_model);

				//为什么一起乘就不对？？?
				//glm::vec4 test=/*glm::vec3(*/text_model_for_front*glm::vec4(this->mark_text_loc_positions[i], 1.0f)/*)*/;
				//std::cout << "test0:" << "x:" << test.x << " y:" << test.y << " z:" << test.z <<" w:"<<test.w<< std::endl;

				//glm::vec4 test1=this->model*test;
				//std::cout << "test1:" << "x:" << test1.x << " y:" << test1.y << " z:" << test1.z << " w:" << test1.w << std::endl;

				//glm::vec4 test2 = this->model*(text_model_for_front*glm::vec4(this->mark_text_loc_positions[i], 1.0f));
				//std::cout << "test2:" << "x:" << test2.x << " y:" << test2.y << " z:" << test2.z << " w:" << test2.w << std::endl;

				//glm::vec4 test3 = (this->model*text_model_for_front)*glm::vec4(this->mark_text_loc_positions[i], 1.0f);
				//std::cout << "test3:" << "x:" << test3.x << " y:" << test3.y << " z:" << test3.z << " w:" << test3.w << std::endl;

				//glm::vec4 test4 = text_model*glm::vec4(this->mark_text_loc_positions[i], 1.0f);
				//std::cout << "test4:" << "x:" << test4.x << " y:" << test4.y << " z:" << test4.z << " w:" << test4.w << std::endl;

				//glm::vec3 test5 = glm::normalize(glm::inverse(glm::transpose(glm::inverse(glm::mat3(this->model))))*glm::vec3(0.0f,-1.0f,0.0f));
				//std::cout << "test5:" << "x:" << test5.x << " y:" << test5.y << " z:" << test5.z << std::endl;


				//glm::vec4 text4 = recover_translate*rotate_for_front*glm::vec4(this->mark_text_loc_positions[i], 1.0f);
				//glm::vec4 text5 = recover_translate*(rotate_for_front*glm::vec4(this->mark_text_loc_positions[i], 1.0f));

				//if (text4 != text5)
				//{
				//	std::cout << "error!" << std::endl;
				//}
			}
		}
		void SetModel(const glm::mat4& model)//调整整体
		{
			this->model = model;

			coordinate_line.SetModel(model*coordinate_line.GetModel());//对坐标轴来说model就是他的model
			for (auto &mark_text : this->mark_texts)
			{
				mark_text.SetModel(model*mark_text.GetModel());//对text来说要先旋转后再使用整体model
			}
		}
		void Show()
		{
			std::cout << "Mark_Text:" << std::endl;
			for (auto &mark_text : this->mark_texts)
			{
				mark_text.Show();
			}
		}
		void Draw(ShaderProgram& line_shader, ShaderProgram& font_shader)
		{
			line_shader.use();
			coordinate_line.Draw(line_shader);

			font_shader.use();
			for (auto &i : mark_texts)
			{
				i.Draw(font_shader);
			}
		}
		
	private:
		glm::vec3 GetTextWorUp(const glm::vec3& text_to_camera_wor, const glm::vec3& camera_up)
		{
			float x_y = 
				-(
				text_to_camera_wor.x*camera_up.x 
				+ text_to_camera_wor.y*camera_up.y 
				+ text_to_camera_wor.z*camera_up.z
				)
				/
				(
					text_to_camera_wor.x*text_to_camera_wor.x
					+ text_to_camera_wor.y*text_to_camera_wor.y 
					+ text_to_camera_wor.z*text_to_camera_wor.z
				);
			glm::vec3 text_wor_up = glm::normalize(x_y*text_to_camera_wor + camera_up);
			return text_wor_up;
		}

		LineObject coordinate_line;
		std::vector<MeshObject> mark_texts;//每个text需要不同的model
		std::vector<glm::vec3> mark_text_loc_positions;

		glm::mat4 model;//整体的model
		//line msg
		//std::vector<Vertex> vertices;
		//std::vector<unsigned int> indices;
	};
}