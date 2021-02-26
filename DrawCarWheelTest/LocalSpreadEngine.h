//#ifndef LOCALENGLINE_H
//#define LOCALENGLINE_H
#pragma once
#include <vector>

#include <glm\glm.hpp>

#include "Mesh.h"
#include "Line.h"
#include "ShaderProgram.h"
#include "CoordinateObject.h"
#include "PickMsg.h"
#include "Range.h"
namespace Engine {
	class LocalSpreadEngine
	{
	public:
		LocalSpreadEngine()
		{
			currentWheelId = -1;
		}
		void SetWheelFlangeTreadModel(int current_wheel_id,glm::mat4 model);

		void AddWheel(const OpenglObj::Mesh& mesh, glm::mat4 model = glm::mat4(1.0f), glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
		void AddWheel(const Object::MeshObject& obj);
		void AddRuler(const Object::CoordinateObject& obj);
		void AddCoordinate(const Object::CoordinateObject& obj);
		void AddLines(const Object::LineObject& obj);
		void UpdateTextDirection(const glm::vec3& camera_position, const glm::vec3& camera_up, const glm::mat4& projection, const glm::mat4& view);

		int GetWheelsSize();
		void DrawRulers(ShaderProgram &line_shader, ShaderProgram &font_shader);
		void DrawCoordinates(ShaderProgram &line_shader, ShaderProgram &font_shader);
		void DrawWheel(ShaderProgram &shader, int id);
		void DrawLines(ShaderProgram &shader);
		//pick操作,接受拾取平面的范围
		PickMsg RayPick_WheelSpread(double xpos, double ypos, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPosition, int scrWidth, int scrHeight, Range widthPickRange, Range lengthPickRange);
	private:
		glm::vec3 GetRayDirection(double nds_x, double nds_y, glm::mat4 projection, glm::mat4 view);
		glm::vec4 GetTreadIntersectPositionAndDisForWheelSpread(glm::vec3 ray_dir, glm::vec3 cameraPosition, glm::vec3 V1, glm::vec3 V2, glm::vec3 V3, glm::mat4 model, Range widthPickRange, Range lengthPickRange);//接收世界坐标

		std::vector<Object::MeshObject> wheel_flange_treads;//长度要该
		std::vector<Object::CoordinateObject> rulers;//ruler的长度和coordinate的长度无需改变
		std::vector<Object::CoordinateObject> coordinates;
		std::vector<Object::LineObject> lines;
		int currentWheelId;
	};
}