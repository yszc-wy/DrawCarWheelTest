#pragma once
#include <vector>

#include <glm\glm.hpp>

#include "MeshObject.h"
#include "ShaderProgram.h"

enum WheelSide {
    A, B
};
class Train //每个wheel 单独掌握一个wheel group
{
public:
	//wheel
	static const float kWheelDistanceBetweenSet;
	static const float kWheelDistanceBetweenPivot;
	static const float kWheelDistanceBetweenFrame;
	static const float kWheelDistanceBetweenBox;

	//frame
	static const float kFrameDistanceInBox;
	static const float kFrameDistanceBetweenBox;

	//box
	static const float kBoxDistance;

	static const int kWheelCount;
	static const float kWheelRadius;
	static const float kWheelWidth;

	static const float kFrameLength;
	static const float kFrameHigth;
	static const float kFrameWidth;
public:
	Train():
		wheel(), 
		wheel_models(),
		train_frame(),
		train_frame_models(),
		train_head(),
		train_head_models(),
		train_box(), 
		train_box_models(),
		marshalling(3),
		activate_wheel_id(-1){}

	void set_wheel(const Object::MeshObject& obj);
	void set_train_head(const Object::MeshObject& obj);
	void set_train_box(const Object::MeshObject& obj);
	void set_train_frame(const Object::MeshObject& obj);
	void set_marshalling(int marshalling);
	int get_marshalling() const;
	void set_activate_wheel_id(int id);

	void BuildAllModel();
	void ClearAll();

	//void ActivateWheel(size_t id, int outputId);

	//void FreezeWheel(size_t id);
	//void FreezeAllWheel();
	void Draw(ShaderProgram& shader, bool train_box_visible);
	int GetOutPutID(size_t id);
	int get_frame_model_size() const;
	glm::mat4 get_frame_model(int id) const;
	
private:
	void BuildWheelModelAndActiveList();
	void BuildFrameModel();
	void BuildBoxModel();

	void BuildLineWheel(const glm::mat4& scale, glm::mat4 translate, const glm::mat4& rotate);
	void BuildLineWheelInBox(const glm::mat4& scale, glm::mat4& translate, const glm::mat4& rotate);

	void BuildFrame(const glm::mat4& scale, glm::mat4 translate);
	void BuildFrameInBox(const glm::mat4& scale, glm::mat4& translate);


	Object::MeshObject wheel;
	std::vector<glm::mat4> wheel_models;
	//std::vector<int> wheels_active_list;

	Object::MeshObject train_frame;
	std::vector<glm::mat4> train_frame_models;

	Object::MeshObject train_head;
	std::vector<glm::mat4> train_head_models;

	Object::MeshObject train_box;
	std::vector<glm::mat4> train_box_models;

	int marshalling;

	int activate_wheel_id;
};

