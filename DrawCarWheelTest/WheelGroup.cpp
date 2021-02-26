#include "WheelGroup.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
//以下距离只是相对于轮子来看的，即轮子之间的距离,非真实距离.
const float WheelGroup::WheelSetDistance = 1274.0f;//轮对距离
const float WheelGroup::WheelPivotDistance = 2500.0f;//轮轴距离
const float WheelGroup::TrainFrameDistance = 7500.0f;//车架距离
const float WheelGroup::CarriageDistance = 3000.0f;//车厢距离
const int WheelGroup::WheelCount = 48;
const float WheelGroup::WheelRadius = 450.0f;
const float WheelGroup::WheelWidth = 135.0f;
int WheelGroup::Size() const
{
	return wheelsModel.size();
}
glm::mat4 WheelGroup::GetModel(int id) const
{
	if (id < 0 || id >= wheelsModel.size())
	{
		return glm::mat4(FLT_MAX);
	}
	else
	{
		return wheelsModel[id];
	}
}
void WheelGroup::SetWheel(const Object& obj)
{
	this->wheel = obj;
}
void WheelGroup::BuildModelAndActiveList()
{
	glm::mat4 scale(1.0f);
	scale = glm::scale(scale, glm::vec3(0.001f, 0.001f, 0.001f));

	glm::mat4 rotate(1.0f);
	rotate = glm::rotate(rotate, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 translate(1.0f);
	translate = glm::translate(translate, glm::vec3(1500.0f - 15500 * 4, -465.0f-1747.77f, -1712.5f));//-1712.5f
	this->BuildLineWheel(scale, translate, rotate);

	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f))*rotate;
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1274.0f))*translate;
	this->BuildLineWheel(scale, translate, rotate);


	//glm::translate(glm::mat4(1.0f), glm::vec3(1500.f - 15500.0f * 4.0f + 1250.0f, -465.0f - 1747.77f, -900.5f));
	//glm::translate(glm::mat4(1.0f), glm::vec3(1500.0f + 15500.0f * 2.0f - 1250.0f, -465.0f - 1747.77f, -900.5f));

}

void WheelGroup::Draw(ShaderProgram& shader)
{
	if (wheelsModel.size() == 0||wheel.MeshSize()==0)
	{
		std::cout << "ERROR::WHEELGROUP::TRY TO DRAW WHEELGROUP WHICH WHEELSMODEL OR WHEEL OBJECT IS NOT INIT!" << std::endl;
	}
	else
	{
		int wheelsModelSize = wheelsModel.size();
		for (int i = 0; i != wheelsModelSize; ++i)
		{
			shader.setInt("wheelID", i);
			if (wheelsActiveList[i] != -1)
			{
				shader.setBool("wheelActivity", true);
			}
			else
			{
				shader.setBool("wheelActivity", false);
			}
			wheel.SetModel(wheelsModel[i]);
			wheel.Draw(shader);
		}
	}
}

void WheelGroup::BuildLineWheel(const glm::mat4& scale, glm::mat4 translate,const glm::mat4& rotate)
{
	for (int i = 0; i != 6; ++i)
	{
		this->BuildLineWheelCarriage(scale, translate, rotate);
		translate = glm::translate(glm::mat4(1.0f), glm::vec3(CarriageDistance, 0.0f, 0.0f))*translate;
	}
}
void WheelGroup::BuildLineWheelCarriage(const glm::mat4& scale, glm::mat4& translate,const glm::mat4& rotate)
{
	wheelsModel.push_back(scale*translate*rotate);
	wheelsActiveList.push_back(-1);

	translate = glm::translate(glm::mat4(1.0f), glm::vec3(WheelPivotDistance, 0.0f, 0.0f))*translate;
	wheelsModel.push_back(scale*translate*rotate);
	wheelsActiveList.push_back(-1);

	translate = glm::translate(glm::mat4(1.0f), glm::vec3(TrainFrameDistance, 0.0f, 0.0f))*translate;
	wheelsModel.push_back(scale*translate*rotate);
	wheelsActiveList.push_back(-1);

	translate = glm::translate(glm::mat4(1.0f), glm::vec3(WheelPivotDistance, 0.0f, 0.0f))*translate;
	wheelsModel.push_back(scale*translate*rotate);
	wheelsActiveList.push_back(-1);
}

void WheelGroup::ActivateWheel(size_t id, int outputId)
{
	if (this->wheelsActiveList.size() == 0)
	{
		std::cout << "ERROR::WHEELGROUP::TRY TO SET ACTIVE LIST WHICH IS NOT INIT!" << std::endl;
	}
	else
	{
		wheelsActiveList[id] = outputId;
	}
}
void WheelGroup::FreezeWheel(size_t id)
{
	if (this->wheelsActiveList.size() == 0)
	{
		std::cout << "ERROR::WHEELGROUP::TRY TO SET ACTIVE LIST WHICH IS NOT INIT!" << std::endl;
	}
	else
	{
		wheelsActiveList[id] = -1;
	}
}
void WheelGroup::FreezeAllWheel()
{
	if (this->wheelsActiveList.size() == 0)
	{
		std::cout << "ERROR::WHEELGROUP::TRY TO SET ACTIVE LIST WHICH IS NOT INIT!" << std::endl;
	}
	else
	{
		for (auto &i : wheelsActiveList)
		{
			i = -1;
		}
	}
}

int WheelGroup::GetOutPutID(size_t id)
{
	if (this->wheelsActiveList.size() == 0)
	{
		std::cout << "ERROR::WHEELGROUP::TRY TO GET ACTIVE LIST WHICH IS NOT INIT!" << std::endl;
		return -1;
	}
	else if (id < 0 || id >= this->wheelsActiveList.size())
	{
		std::cout << "ERROR::WHEELGROUP::TRY TO GET ACTIVE LIST USE ILLEGAL ID!" << std::endl;
		return -1;
	}
	else
	{
		return this->wheelsActiveList[id];
	}
}