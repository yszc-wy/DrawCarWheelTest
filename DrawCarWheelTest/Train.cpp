#include "Train.h"
#include <iostream>

#include <qopenglfunctions_3_3_compatibility.h>
#include <glm/gtc/matrix_transform.hpp>

//以下距离只是相对于轮子来看的，即轮子之间的距离,非真实距离.
const float Train::kWheelDistanceBetweenSet = 1274.0f;
const float Train::kWheelDistanceBetweenPivot = 2500.0f;
const float Train::kWheelDistanceBetweenFrame = 7500.0f;
const float Train::kWheelDistanceBetweenBox = 3000.0f;

const float Train::kFrameDistanceInBox = kWheelDistanceBetweenFrame + kWheelDistanceBetweenPivot;
const float Train::kFrameDistanceBetweenBox = kWheelDistanceBetweenBox + kWheelDistanceBetweenPivot;

const float Train::kBoxDistance = kWheelDistanceBetweenFrame + 2 * kWheelDistanceBetweenPivot + kWheelDistanceBetweenBox;

const int Train::kWheelCount = 48;
const float Train::kWheelRadius = 450.0f;
const float Train::kWheelWidth = 135.0f;

const float Train::kFrameLength = 3500.0f;
const float Train::kFrameHigth = 580.0f;
const float Train::kFrameWidth = 2000.0f;
int Train::get_frame_model_size() const
{
	return this->train_frame_models.size();
}
glm::mat4 Train::get_frame_model(int id) const
{
	if (id < 0 || id >= this->train_frame_models.size())
	{
		return glm::mat4(FLT_MAX);
	}
	else
	{
		return this->train_frame_models[id];
	}
}
int Train::get_marshalling()const
{
	return this->marshalling;
}
void Train::set_wheel(const Object::MeshObject& obj)
{
	this->wheel = obj;
}
void Train::set_train_head(const Object::MeshObject& obj)
{
	this->train_head = obj;
}
void Train::set_train_box(const Object::MeshObject& obj)
{
	this->train_box = obj;
}
void Train::set_train_frame(const Object::MeshObject& obj)
{
	this->train_frame = obj;
}
void Train::set_marshalling(int marshalling)
{
	if (marshalling < 2)
	{
		std::cout << "Error::WheelGroup::set_marshalling::illegal marshalling" << std::endl;
		return;
	}
	this->marshalling = marshalling;

}
void Train::BuildAllModel()
{
	this->ClearAll();
	this->BuildWheelModelAndActiveList();
	this->BuildFrameModel();
	this->BuildBoxModel();
}
void Train::ClearAll()
{
	this->wheel_models.clear();
	//this->wheels_active_list.clear();
	this->train_frame_models.clear();
	this->train_head_models.clear();
	this->train_box_models.clear();
}
void Train::BuildWheelModelAndActiveList()
{
	glm::mat4 scale(1.0f);
	scale = glm::scale(scale, glm::vec3(0.001f, 0.001f, 0.001f));

	glm::mat4 rotate(1.0f);
	rotate = glm::rotate(rotate, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 translate(1.0f);
	translate = glm::translate(translate, glm::vec3(/*1500.0f - 15500 * 4*/-kWheelDistanceBetweenPivot/2.0f, -/*-1747.77f*/0.0f, /*-1712.5f*/-kWheelDistanceBetweenSet/2.0f));//-1712.5f
	this->BuildLineWheel(scale, translate, rotate);

	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f))*rotate;
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, kWheelDistanceBetweenSet))*translate;
	this->BuildLineWheel(scale, translate, rotate);


	//glm::translate(glm::mat4(1.0f), glm::vec3(1500.f - 15500.0f * 4.0f + 1250.0f, -465.0f - 1747.77f, -900.5f));
	//glm::translate(glm::mat4(1.0f), glm::vec3(1500.0f + 15500.0f * 2.0f - 1250.0f, -465.0f - 1747.77f, -900.5f));

}
void Train::BuildLineWheel(const glm::mat4& scale, glm::mat4 translate, const glm::mat4& rotate)
{
	for (int i = 0; i != this->marshalling; ++i)
	{
		this->BuildLineWheelInBox(scale, translate, rotate);
		translate = glm::translate(glm::mat4(1.0f), glm::vec3(kWheelDistanceBetweenBox, 0.0f, 0.0f))*translate;
	}
}
void Train::BuildLineWheelInBox(const glm::mat4& scale, glm::mat4& translate, const glm::mat4& rotate)//一车厢的列轮子
{
	wheel_models.push_back(scale*translate*rotate);
	//wheels_active_list.push_back(-1);

	translate = glm::translate(glm::mat4(1.0f), glm::vec3(kWheelDistanceBetweenPivot, 0.0f, 0.0f))*translate;
	wheel_models.push_back(scale*translate*rotate);
	//wheels_active_list.push_back(-1);

	translate = glm::translate(glm::mat4(1.0f), glm::vec3(kWheelDistanceBetweenFrame, 0.0f, 0.0f))*translate;
	wheel_models.push_back(scale*translate*rotate);
	//wheels_active_list.push_back(-1);

	translate = glm::translate(glm::mat4(1.0f), glm::vec3(kWheelDistanceBetweenPivot, 0.0f, 0.0f))*translate;
	wheel_models.push_back(scale*translate*rotate);
	//wheels_active_list.push_back(-1);
}

void Train::BuildFrameModel()
{
	glm::mat4 scale(1.0f);
	scale= glm::scale(glm::mat4(1.0f), glm::vec3(0.001f, 0.001f, 0.001f));  //总体缩放

	glm::mat4 translate(1.0f);
	//translate = glm::translate(translate, glm::vec3(1500.0f - 15500 * 4, -465.0f - 1747.77f, -1712.5f));//-1712.5f

	this->BuildFrame(scale, translate);
}

void Train::BuildFrame(const glm::mat4& scale, glm::mat4 translate)
{
	for (int i = 0; i != this->marshalling; ++i)
	{
		this->BuildFrameInBox(scale, translate);
		translate = glm::translate(glm::mat4(1.0f), glm::vec3(kFrameDistanceBetweenBox, 0.0f, 0.0f))*translate;
	}
}

void Train::BuildFrameInBox(const glm::mat4& scale, glm::mat4& translate)//传指针
{
	this->train_frame_models.push_back(scale*translate);
	//wheels_active_list.push_back(-1);

	translate = glm::translate(glm::mat4(1.0f), glm::vec3(kFrameDistanceInBox, 0.0f, 0.0f))*translate;
	this->train_frame_models.push_back(scale*translate);
	//wheels_active_list.push_back(-1);
}

void Train::BuildBoxModel()
{
	glm::mat4 scale(1.0f);
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.001f, 0.001f, 0.001f));  //总体缩放

	glm::mat4 translate(1.0f);
	translate = glm::translate(translate, glm::vec3(kWheelDistanceBetweenFrame/2.0f+kWheelDistanceBetweenPivot/2.0f,475.0f, 0.0f));//-1712.5f

	//车头
	this->train_head_models.push_back(scale*translate);
	//车身
	for (int i = 0; i != this->marshalling - 2;++i)
	{
		translate = glm::translate(glm::mat4(1.0f), glm::vec3(kBoxDistance, 0.0f, 0.0f))*translate;
		this->train_box_models.push_back(scale*translate);
	}
	//车尾
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(kBoxDistance, 0.0f, 0.0f))*translate;
	glm::mat4 rotate(1.0f);
	rotate = glm::rotate(rotate, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->train_head_models.push_back(scale*translate*rotate);
}

//void Train::ActivateWheel(size_t id, int outputId)
//{
//	if (this->wheels_active_list.size() == 0)
//	{
//		std::cout << "ERROR::WHEELGROUP::TRY TO SET ACTIVE LIST WHICH IS NOT INIT!" << std::endl;
//	}
//	else
//	{
//		wheels_active_list[id] = outputId;
//	}
//}
void Train::set_activate_wheel_id(int id)
{
	this->activate_wheel_id = id;
}
//void Train::FreezeWheel(size_t id)
//{
//	if (this->wheels_active_list.size() == 0)
//	{
//		std::cout << "ERROR::WHEELGROUP::TRY TO SET ACTIVE LIST WHICH IS NOT INIT!" << std::endl;
//	}
//	else
//	{
//		wheels_active_list[id] = -1;
//	}
//}
//void Train::FreezeAllWheel()
//{
//	if (this->wheels_active_list.size() == 0)
//	{
//		std::cout << "ERROR::WHEELGROUP::TRY TO SET ACTIVE LIST WHICH IS NOT INIT!" << std::endl;
//	}
//	else
//	{
//		for (auto &i : wheels_active_list)
//		{
//			i = -1;
//		}
//	}
//}

//int Train::GetOutPutID(size_t id)
//{
//	if (this->wheels_active_list.size() == 0)
//	{
//		std::cout << "ERROR::WHEELGROUP::TRY TO GET ACTIVE LIST WHICH IS NOT INIT!" << std::endl;
//		return -1;
//	}
//	else if (id < 0 || id >= this->wheels_active_list.size())
//	{
//		std::cout << "ERROR::WHEELGROUP::TRY TO GET ACTIVE LIST USE ILLEGAL ID!" << std::endl;
//		return -1;
//	}
//	else
//	{
//		return this->wheels_active_list[id];
//	}
//}



void Train::Draw(ShaderProgram& shader,bool train_box_visible)
{
	if(!train_box_visible)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (this->train_head_models.size() == 0 || train_head.MeshSize() == 0)
	{
		std::cout << "ERROR::WHEELGROUP::TRY TO DRAW WHEELGROUP WHICH train_head_models OR train_head OBJECT IS NOT INIT!" << std::endl;
	}
	else
	{
		shader.setBool("wheelActivity", false);
		int train_head_model_size = train_head_models.size();
		for (int i = 0; i != train_head_model_size; ++i)
		{
			this->train_head.SetModel(this->train_head_models[i]);
			this->train_head.Draw(shader);
		}
	}

	if (this->train_box_models.size() == 0 || train_box.MeshSize() == 0)
	{
		std::cout << "ERROR::WHEELGROUP::TRY TO DRAW WHEELGROUP WHICH train_box_models OR train_box OBJECT IS NOT INIT!" << std::endl;
	}
	else
	{
		shader.setBool("wheelActivity", false);
		int train_box_model_size = train_box_models.size();
		for (int i = 0; i != train_box_model_size; ++i)
		{
			this->train_box.SetModel(this->train_box_models[i]);
			this->train_box.Draw(shader);
		}
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (train_frame_models.size() == 0 || train_frame.MeshSize() == 0)
	{
		std::cout << "ERROR::WHEELGROUP::TRY TO DRAW WHEELGROUP WHICH train_frame_models OR train_frame OBJECT IS NOT INIT!" << std::endl;
	}
	else
	{
		int train_frame_model_size = train_frame_models.size();
		for (int i = 0; i != train_frame_model_size; ++i)
		{
			shader.setBool("wheelActivity", false);

			this->train_frame.SetModel(this->train_frame_models[i]);
			this->train_frame.Draw(shader);
		}
	}

	if (wheel_models.size() == 0 || wheel.MeshSize() == 0)
	{
		std::cout << "ERROR::WHEELGROUP::TRY TO DRAW WHEELGROUP WHICH WHEELSMODEL OR WHEEL OBJECT IS NOT INIT!" << std::endl;
	}
	else
	{
		shader.setBool("wheelActivity", false);
		int wheelsModelSize = wheel_models.size();
		for (int i = 0; i != wheelsModelSize; ++i)
		{
			//shader.setInt("wheelID", i);
			if (this->activate_wheel_id==i)
			{
				shader.setBool("wheelActivity", true);
			}
			else
			{
				shader.setBool("wheelActivity", false);
			}
			wheel.SetModel(this->wheel_models[i]);
			wheel.Draw(shader);
		}
	}
}