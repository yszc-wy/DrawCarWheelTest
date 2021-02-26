#pragma once
#include <vector>
#include "Mesh.h"
#include "ShaderProgram.h"
#include <glm\glm.hpp>
#include "Object.h"
#include "WheelGroup.h"
class WheelPickMsg {
public:
	WheelPickMsg(float angle,float width):angle(angle),width(width){}

	float angle;
	float width;
};
class Engine
{
public:
	Engine(){}
	void AddMesh(const Mesh& mesh,glm::mat4 model=glm::mat4(1.0f));
	void AddObject(const Object& obj);

	void SetWheelGroup(const Object& obj);
	void DrawWheelGroup(ShaderProgram &shader);

	int GetObjectsSize();
	void Draw(ShaderProgram &shader);
	void DrawSingle(ShaderProgram &shader,int id);
	//pick操作
	WheelPickMsg RayPick_Wheel(double xpos, double ypos, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPosition, int scrWidth, int scrHeight);
	int RayPick_WheelGroup(double xpos, double ypos, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPosition, int scrWidth, int scrHeight);
	
	void ActivateWheel(WheelSide side, int id, int outputId);
	void ActivateWheel(int id, int outputId);
	void FreezeWheel(WheelSide side, int id);
	void FreezeAllWheel();
private:
	glm::vec3 GetRayDirection(double nds_x, double nds_y, glm::mat4 projection, glm::mat4 view);
	glm::vec4 GetPlaneIntersectPositionAndDis(glm::vec3 ray_dir, glm::vec3 cameraPosition, glm::vec3 V1, glm::vec3 V2, glm::vec3 V3, glm::vec3 rLoc);//接收世界坐标
	glm::vec4 GetTreadIntersectPositionAndDisForWheel(glm::vec3 ray_dir, glm::vec3 cameraPosition, glm::vec3 V1,glm::vec3 rLoc, glm::mat4 model, float widthStart, float widthEnd);//接收世界坐标
	glm::vec4 GetTreadIntersectPositionAndDisForWheelGroup(glm::vec3 ray_dir, glm::vec3 cameraPosition, glm::vec3 V1, glm::vec3 rLoc, glm::mat4 model, float widthStart, float widthEnd);
	float IntersectWithCylinderForWheelGroup(const glm::mat4& model,/*const glm::vec3& cylinderCenter,*/float cylinderWidth,float cylinderRadius,const glm::vec3& ray_dir,const glm::vec3& cameraPosition);
	
	std::vector<Object> objects;
	WheelGroup wheelGroup;
};