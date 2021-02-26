#include "LocalSpreadEngine.h"

#include <iostream>
#include <string>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "Engine_inl.h"
namespace Engine {
	void LocalSpreadEngine::AddWheel(const Object::MeshObject& obj)
	{
		if (this->wheel_flange_treads.size() != 0)
		{
			this->wheel_flange_treads[0].DeleteMeshs();
			this->wheel_flange_treads.clear();
		}

		this->wheel_flange_treads.push_back(obj);
	}
	void LocalSpreadEngine::AddWheel(const OpenglObj::Mesh& mesh, glm::mat4 model, glm::vec3 color)
	{
		if (this->wheel_flange_treads.size() != 0)
		{
			this->wheel_flange_treads[0].DeleteMeshs();
			this->wheel_flange_treads.clear();
		}

		Object::MeshObject obj(mesh, model, color);
		this->wheel_flange_treads.push_back(obj);
	}
	void LocalSpreadEngine::AddRuler(const Object::CoordinateObject& obj)
	{
		this->rulers.push_back(obj);
	}
	void LocalSpreadEngine::AddCoordinate(const Object::CoordinateObject& obj)
	{
		this->coordinates.push_back(obj);
	}
	void LocalSpreadEngine::DrawRulers(ShaderProgram &line_shader,ShaderProgram &font_shader)
	{
		//std::cout << "rule:" << std::endl;
		//std::cout << this->rulers.size() << std::endl;
		for (auto &ruler : this->rulers)
		{
			ruler.Draw(line_shader, font_shader);
		}
	}
	void LocalSpreadEngine::DrawCoordinates(ShaderProgram &line_shader, ShaderProgram &font_shader)
	{
		//std::cout << "coordinate:" << std::endl;
		//std::cout << this->coordinates.size() << std::endl;
		for (auto &coordinate : this->coordinates)
		{
			coordinate.Draw(line_shader, font_shader);
		}
	}
	void LocalSpreadEngine::DrawLines(ShaderProgram &shader)
	{
		for (auto &line : this->lines)
		{
			line.Draw(shader);
		}
	}
	void LocalSpreadEngine::AddLines(const Object::LineObject& obj)
	{
		this->lines.push_back(obj);
	}
	void LocalSpreadEngine::UpdateTextDirection(const glm::vec3& camera_position, const glm::vec3& camera_up, const glm::mat4& projection, const glm::mat4& view)
	{
		for (auto &ruler : this->rulers)
		{
			ruler.UpdateTextDirection(camera_position, camera_up, projection, view);
		}
		for (auto &coordinate : this->coordinates)
		{
			coordinate.UpdateTextDirection(camera_position, camera_up, projection, view);
		}
	}

	//void LocalEngine::SetCurrentWheel(int id)
	//{
	//	if (id < 0 || id >= ranges.size())
	//	{
	//		std::cout << "ERROR::LOCALENGINE::ILLEGAL CURRENT WHEEL ID!" << std::endl;
	//	}
	//	else
	//		this->currentWheelId = id;
	//}
	//void LocalEngine::AddWheelMesh(const Mesh& mesh, const TreadRange& range, glm::mat4 model)
	//{
	//	Object obj(mesh, model);
	//	objects.push_back(obj);
	//	ranges.push_back(range);
	//}
	int LocalSpreadEngine::GetWheelsSize()
	{
		return this->wheel_flange_treads.size();
	}
	void LocalSpreadEngine::DrawWheel(ShaderProgram &shader, int id)
	{
		if (id >= this->wheel_flange_treads.size() || id < 0)
		{
			std::cout << "ERROR::ENGINE::DRAWSINGLE::ILLEGAL ID!" << std::endl;
		}
		else
		{
			this->wheel_flange_treads[id].Draw(shader);
		}
	}

	void LocalSpreadEngine::SetWheelFlangeTreadModel(int current_wheel_id, glm::mat4 model)
	{
		int wheel_size = this->wheel_flange_treads.size();
		if (current_wheel_id < 0 || current_wheel_id >= wheel_size)
		{
			std::cout << "Error::LocalSpreadEngine::SetWheelFlangeTreadModel::illegal current_wheel_id" << std::endl;
		}
		this->wheel_flange_treads[current_wheel_id].SetModel(model);
	}


	PickMsg LocalSpreadEngine::RayPick_WheelSpread(double xpos, double ypos, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPosition, int scrWidth, int scrHeight, Range widthPickRange, Range lengthPickRange)
	{
		//std::cout << "RayPick_WheelSpread 1" << std::endl;

		float nds_x = (2.0f * xpos) / scrWidth - 1.0f;//缩放到-1,0f-1.0f
		float ndx_y = 1.0f - (2.0f * ypos) / scrHeight;
		//float z = -1.0f;//投影到远裁剪平面
		glm::vec3 ray_dir = this->GetRayDirection(nds_x, ndx_y, projection, view);

		//针对object0
		auto &object = this->wheel_flange_treads[0];

		glm::vec3 P1V1Loc(0.0f, 0.0f, 0.0f);  //平面1  v1要是圆点
		glm::vec3 P1V2Loc(1.0f, 0.0f, 0.0f);
		glm::vec3 P1V3Loc(0.0f, 0.0f, 1.0f);

		glm::vec3 P1V1Wor = glm::vec3(object.GetModel()*glm::vec4(P1V1Loc, 1.0f));
		glm::vec3 P1V2Wor = glm::vec3(object.GetModel()*glm::vec4(P1V2Loc, 1.0f));
		glm::vec3 P1V3Wor = glm::vec3(object.GetModel()*glm::vec4(P1V3Loc, 1.0f));

		glm::vec4 P1PD = this->GetTreadIntersectPositionAndDisForWheelSpread(ray_dir, cameraPosition, P1V1Wor, P1V2Wor, P1V3Wor, object.GetModel(), widthPickRange, lengthPickRange);

		//std::cout << "msg: " << P1PD.w<<" "<<P2PD.w<<" "<<TPD.w<<std::endl;
		if (P1PD.w == FLT_MAX)
		{
			//std::cout << "no thing!" << std::endl;
			return PickMsg(-1, -1);
		}
		else//
		{
			glm::vec4 pickWor = glm::vec4(glm::vec3(P1PD), 1.0f);
			glm::vec3 pickLoc = glm::vec3(glm::inverse(object.GetModel())*pickWor);

			float x = pickLoc.x;
			float z = pickLoc.z;

			return PickMsg(x, z);
		}
		//std::cout << "RayPick_WheelSpread 2" << std::endl;
	}

	glm::vec3 LocalSpreadEngine::GetRayDirection(double nds_x, double nds_y, glm::mat4 projection, glm::mat4 view)
	{
		glm::vec3 ray_nds = glm::vec3(nds_x, nds_y, -1.0f);
		glm::vec4 ray_clip = glm::vec4(ray_nds, 1.0f);//裁剪坐标

													  //camera
		glm::vec4 ray_camera = glm::inverse(projection)*ray_clip;
		ray_camera = glm::vec4(ray_camera.x, ray_camera.y, -1.0, 0.0);//没有裁剪(标准化)的摄像机坐标，所以把射线目的坐标z轴设为-1
																	  //world
		glm::vec3 ray_world = glm::normalize(glm::vec3(glm::inverse(view)*ray_camera));//不需要cameraPosition，view中就包含cameraPosition的信息，相当于直接转化射线

		glm::vec3 ray_dir = glm::normalize(glm::vec3(ray_world));

		//std::cout << "ray_world:" << ray_world.x << " " << ray_world.y << " " << ray_world.z << std::endl;


		return ray_dir;
	}



	glm::vec4 LocalSpreadEngine::GetTreadIntersectPositionAndDisForWheelSpread(glm::vec3 ray_dir, glm::vec3 cameraPosition, glm::vec3 V1, glm::vec3 V2, glm::vec3 V3, glm::mat4 model, Range widthPickRange, Range lengthPickRange)
	{
		glm::vec3 dv1 = V2 - V1;
		glm::vec3 dv2 = V3 - V1;
		glm::mat3 coefficient = glm::mat3(-ray_dir, dv1, dv2);

		glm::vec3 value = cameraPosition - V1;
		glm::vec3 solution = glm::inverse(coefficient)*value;

		//std::cout <<"solution: "<<solution.x << std::endl;
		/*if (isinf(solution.x))
		{
		return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
		}
		else*/ if (solution.x > 0)//防止反向射到
		{
			float u = solution.y;
			float v = solution.z;
			//if (solution.x > 0.9)
			//{
			//std::cout <<"len:"<< solution.x <<" u:"<<u <<" v:"<< v << std::endl;
			//}
			glm::vec3 pickPoint = V1 + u*dv1 + v*dv2;
			glm::vec4 pickWor = glm::vec4(glm::vec3(pickPoint), 1.0f);
			glm::vec3 pickLoc = glm::vec3(glm::inverse(model)*pickWor);

			if ((pickLoc.x >= lengthPickRange.GetMin() && pickLoc.x <= lengthPickRange.GetMax())&&(pickLoc.z>= widthPickRange.GetMin()&&pickLoc.z<= widthPickRange.GetMax()))//只负责拾取平面的判断
			{
				return glm::vec4(pickPoint, solution.x);
			}
			else
			{
				return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
			}
		}
		else
		{
			return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
		}
	}
}