#include "WorldEngine.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine_inl.h"
namespace Engine {
	//void Engine::addMesh(const Mesh& mesh, glm::mat4 model)
	//{
	//	Object obj(mesh, model);
	//	//std::cout << "initE1" << std::endl;
	//	objects.push_back(obj);
	//	//std::cout << "initE2" << std::endl;
	//}
	void WorldEngine::SetTrain(const Object::MeshObject& wheel, const Object::MeshObject& train_frame,
		const Object::MeshObject& train_box, const Object::MeshObject& train_head, int marshalling)
	{
		this->train.set_wheel(wheel);
		this->train.set_train_frame(train_frame);
		this->train.set_train_box(train_box);
		this->train.set_train_head(train_head);
		this->train.set_marshalling(marshalling);
		this->train.set_activate_wheel_id(-1);
		this->train.BuildAllModel();
	}

	void WorldEngine::DrawTrain(ShaderProgram &shader,bool train_box_visible)
	{
		this->train.Draw(shader, train_box_visible);
	}

	int WorldEngine::RayPick_TrainFrame(double xpos, double ypos, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPosition, int scrWidth, int scrHeight)
	{
		float nds_x = (2.0f * xpos) / scrWidth - 1.0f;//缩放到-1,0f-1.0f
		float ndx_y = 1.0f - (2.0f * ypos) / scrHeight;

		glm::vec3 ray_dir = this->GetRayDirection(nds_x, ndx_y, projection, view);
		int frame_size = train.get_frame_model_size();
		float minDis = FLT_MAX;
		float minI = -1;
		for (int i = 0; i != frame_size; ++i)
		{
			float dis = this->IntersectWithCubeForFrame(train.get_frame_model(i), Train::kFrameLength, Train::kFrameWidth, Train::kFrameHigth,ray_dir, cameraPosition);
			
			if (minDis > dis)
			{
				minDis = dis;
				minI = i;
			}
		}
		//this->ActivateWheel(minI, 0);
		return minI;
	}

	//void WorldEngine::SetTrainFrame(const Object::MeshObject& obj)
	//{
	//	this->trainFrame = obj;
	//}
	//void WorldEngine::DrawTrainFrame(ShaderProgram &shader)
	//{
	//	//shader.setMat4("model", this->trainFrame.GetModel());
	//	this->trainFrame.Draw(shader);
	//}
	//void WorldEngine::SetTrainBox(const Object::MeshObject& obj)
	//{
	//	this->trainBox = obj;
	//}
	//void WorldEngine::DrawTrainBox(ShaderProgram &shader)
	//{
	//	//shader.setMat4("model", this->trainBox.GetModel());
	//	this->trainBox.Draw(shader);
	//}

	//void WorldEngine::AddObject(const Object& obj)
	//{
	//	objects.push_back(obj);
	//}
	//void WorldEngine::AddMesh(const Mesh& mesh, glm::mat4 model)
	//{
	//	Object obj(mesh, model);
	//	objects.push_back(obj);
	//}
	//int WorldEngine::GetObjectsSize()
	//{
	//	return objects.size();
	//}

	//void WorldEngine::DrawObjects(ShaderProgram &shader)
	//{
	//	//移动
	//	//int size = objects.size();
	//	//for (int i = 0; i != size; ++i)
	//	//{
	//	//	shader.setMat4("model", objects[i].model);
	//	//	objects[i].mesh.Draw(shader);
	//	//}
	//	for (auto &i : objects)
	//	{
	//		shader.setMat4("model", i.GetModel());
	//		i.Draw(shader);
	//	}
	//	//std::cout << "Draw ok3" << std::endl;
	//}

	float WorldEngine::IntersectWithCubeForFrame(const glm::mat4& model, float cube_length, float cube_width, float cube_hight, const glm::vec3& ray_dir, const glm::vec3& cameraPosition)
	{
		std::vector<glm::vec4> picked_points;

		glm::vec3 P1V1Loc(0.0f, cube_hight / 2.0f, 0.0f);  //平面1  
		glm::vec3 P1V2Loc(1.0f, cube_hight / 2.0f, 0.0f);
		glm::vec3 P1V3Loc(0.0f, cube_hight / 2.0f, 1.0f);
		glm::vec3 P1Range_Min(-cube_length / 2.0f, cube_hight / 2.0f - 1.0f, -cube_width / 2.0f);
		glm::vec3 P1Range_Max(cube_length / 2.0f, cube_hight / 2.0f + 1.0f, cube_width / 2.0f);

		glm::vec3 P1V1Wor = glm::vec3(model*glm::vec4(P1V1Loc, 1.0f));
		glm::vec3 P1V2Wor = glm::vec3(model*glm::vec4(P1V2Loc, 1.0f));
		glm::vec3 P1V3Wor = glm::vec3(model*glm::vec4(P1V3Loc, 1.0f));

		picked_points.push_back(this->GetPlaneIntersectPositionAndDis(ray_dir, cameraPosition, model, P1V1Wor, P1V2Wor, P1V3Wor, P1Range_Min, P1Range_Max));

		glm::vec3 P2V1Loc(0.0f, -cube_hight / 2.0f, 0.0f);  //平面2 
		glm::vec3 P2V2Loc(1.0f, -cube_hight / 2.0f, 0.0f);
		glm::vec3 P2V3Loc(0.0f, -cube_hight / 2.0f, 1.0f);
		glm::vec3 P2Range_Min(-cube_length / 2.0f, -cube_hight / 2.0f - 1.0f, -cube_width / 2.0f);
		glm::vec3 P2Range_Max(cube_length / 2.0f, -cube_hight / 2.0f + 1.0f, cube_width / 2.0f);

		glm::vec3 P2V1Wor = glm::vec3(model*glm::vec4(P2V1Loc, 1.0f));
		glm::vec3 P2V2Wor = glm::vec3(model*glm::vec4(P2V2Loc, 1.0f));
		glm::vec3 P2V3Wor = glm::vec3(model*glm::vec4(P2V3Loc, 1.0f));

		picked_points.push_back(this->GetPlaneIntersectPositionAndDis(ray_dir, cameraPosition, model, P2V1Wor, P2V2Wor, P2V3Wor, P2Range_Min, P2Range_Max));

		glm::vec3 P3V1Loc(0.0f, 0.0f, cube_width/2.0f);  //平面3
		glm::vec3 P3V2Loc(1.0f, 0.0f, cube_width / 2.0f);
		glm::vec3 P3V3Loc(0.0f, 1.0f, cube_width / 2.0f);
		glm::vec3 P3Range_Min(-cube_length / 2.0f, -cube_hight / 2.0f, cube_width / 2.0f-1.0f);
		glm::vec3 P3Range_Max(cube_length / 2.0f, cube_hight / 2.0f, cube_width / 2.0f+1.0f);

		glm::vec3 P3V1Wor = glm::vec3(model*glm::vec4(P3V1Loc, 1.0f));
		glm::vec3 P3V2Wor = glm::vec3(model*glm::vec4(P3V2Loc, 1.0f));
		glm::vec3 P3V3Wor = glm::vec3(model*glm::vec4(P3V3Loc, 1.0f));

		picked_points.push_back(this->GetPlaneIntersectPositionAndDis(ray_dir, cameraPosition, model, P3V1Wor, P3V2Wor, P3V3Wor, P3Range_Min, P3Range_Max));

		glm::vec3 P4V1Loc(0.0f, 0.0f, -cube_width / 2.0f);  //平面4  
		glm::vec3 P4V2Loc(1.0f, 0.0f, -cube_width / 2.0f);
		glm::vec3 P4V3Loc(0.0f, 1.0f, -cube_width / 2.0f);
		glm::vec3 P4Range_Min(-cube_length / 2.0f, -cube_hight / 2.0f, -cube_width / 2.0f - 1.0f);
		glm::vec3 P4Range_Max(cube_length / 2.0f, cube_hight / 2.0f, -cube_width / 2.0f + 1.0f);

		glm::vec3 P4V1Wor = glm::vec3(model*glm::vec4(P4V1Loc, 1.0f));
		glm::vec3 P4V2Wor = glm::vec3(model*glm::vec4(P4V2Loc, 1.0f));
		glm::vec3 P4V3Wor = glm::vec3(model*glm::vec4(P4V3Loc, 1.0f));

		picked_points.push_back(this->GetPlaneIntersectPositionAndDis(ray_dir, cameraPosition, model, P4V1Wor, P4V2Wor, P4V3Wor, P4Range_Min, P4Range_Max));

		glm::vec3 P5V1Loc(cube_length/2.0f, 0.0f, 0.0f);  //平面5 
		glm::vec3 P5V2Loc(cube_length/2.0f, 1.0f, 0.0f);
		glm::vec3 P5V3Loc(cube_length/2.0f, 0.0f, 1.0f);
		glm::vec3 P5Range_Min(cube_length / 2.0f-1.0f, -cube_hight / 2.0f, -cube_width / 2.0f);
		glm::vec3 P5Range_Max(cube_length / 2.0f+1.0f, cube_hight / 2.0f, cube_width / 2.0f);

		glm::vec3 P5V1Wor = glm::vec3(model*glm::vec4(P5V1Loc, 1.0f));
		glm::vec3 P5V2Wor = glm::vec3(model*glm::vec4(P5V2Loc, 1.0f));
		glm::vec3 P5V3Wor = glm::vec3(model*glm::vec4(P5V3Loc, 1.0f));

		picked_points.push_back(this->GetPlaneIntersectPositionAndDis(ray_dir, cameraPosition, model, P5V1Wor, P5V2Wor, P5V3Wor, P5Range_Min, P5Range_Max));

		glm::vec3 P6V1Loc(-cube_length/2.0f, 0.0f, 0.0f);  //平面6
		glm::vec3 P6V2Loc(-cube_length/2.0f, 1.0f, 0.0f);
		glm::vec3 P6V3Loc(-cube_length/2.0f, 0.0f, 1.0f);
		glm::vec3 P6Range_Min(-cube_length / 2.0f - 1.0f, -cube_hight / 2.0f, -cube_width / 2.0f);
		glm::vec3 P6Range_Max(-cube_length / 2.0f + 1.0f, cube_hight / 2.0f, cube_width / 2.0f);

		glm::vec3 P6V1Wor = glm::vec3(model*glm::vec4(P6V1Loc, 1.0f));
		glm::vec3 P6V2Wor = glm::vec3(model*glm::vec4(P6V2Loc, 1.0f));
		glm::vec3 P6V3Wor = glm::vec3(model*glm::vec4(P6V3Loc, 1.0f));

		picked_points.push_back(this->GetPlaneIntersectPositionAndDis(ray_dir, cameraPosition, model, P6V1Wor, P6V2Wor, P6V3Wor, P6Range_Min, P6Range_Max));

		float min_dis=FLT_MAX;
		int min_i =-1;
		for (int i = 0; i != 6; ++i)
		{
			if(min_dis>picked_points[i].w)
			{
				min_dis = picked_points[i].w;
				min_i = i;
			}
		}

		return min_dis;
	}
	float WorldEngine::IntersectWithCylinderForWheel(const glm::mat4& model, /*const glm::vec3& cylinderCenter, */float cylinderWidth, float cylinderRadius, const glm::vec3& ray_dir, const glm::vec3& cameraPosition)//仅限于Z轴方向上的轮子pick
	{

		glm::vec3 P1V1Loc(0.0f, 0.0f, 0.0f);  //平面1  v1要是圆点,原来的面是平躺着的，所有pick面也是躺着的,定制化，不能动
		glm::vec3 P1V2Loc(1.0f, 0.0f, 0.0f);
		glm::vec3 P1V3Loc(0.0f, 0.0f, 1.0f);
		glm::vec3 P1RLoc(cylinderRadius, 0.0f, 0.0f);

		glm::vec3 P2V1Loc(0.0f, 0.0f + cylinderWidth, 0.0f);  //平面2
		glm::vec3 P2V2Loc(1.0f, 0.0f + cylinderWidth, 0.0f);
		glm::vec3 P2V3Loc(0.0f, 0.0f + cylinderWidth, 1.0f);
		glm::vec3 P2RLoc(cylinderRadius, 0.0f + cylinderWidth, 0.0f);

		glm::vec3 P1V1Wor = glm::vec3(model*glm::vec4(P1V1Loc, 1.0f));
		glm::vec3 P1V2Wor = glm::vec3(model*glm::vec4(P1V2Loc, 1.0f));
		glm::vec3 P1V3Wor = glm::vec3(model*glm::vec4(P1V3Loc, 1.0f));
		glm::vec3 P1RWor = glm::vec3(model*glm::vec4(P1RLoc, 1.0f));

		glm::vec4 P1PD = this->GetPlaneIntersectPositionAndDis(ray_dir, cameraPosition, P1V1Wor, P1V2Wor, P1V3Wor, P1RWor);

		glm::vec3 P2V1Wor = glm::vec3(model*glm::vec4(P2V1Loc, 1.0f));
		glm::vec3 P2V2Wor = glm::vec3(model*glm::vec4(P2V2Loc, 1.0f));
		glm::vec3 P2V3Wor = glm::vec3(model*glm::vec4(P2V3Loc, 1.0f));
		glm::vec3 P2RWor = glm::vec3(model*glm::vec4(P2RLoc, 1.0f));

		glm::vec4 P2PD = this->GetPlaneIntersectPositionAndDis(ray_dir, cameraPosition, P2V1Wor, P2V2Wor, P2V3Wor, P2RWor);

		glm::vec4 TPD = this->GetTreadIntersectPositionAndDisForWheelGroup(ray_dir, cameraPosition, P1V1Wor, P1RWor, model, 0, cylinderWidth);

		//std::cout << "msg: " << P1PD.w<<" "<<P2PD.w<<" "<<TPD.w<<std::endl;
		glm::vec4 minPPD = P1PD.w > P2PD.w ? P2PD : P1PD;
		glm::vec4 minPD = TPD.w > minPPD.w ? minPPD : TPD;

		return minPD.w;

	}

	glm::vec3 WorldEngine::GetRayDirection(double nds_x, double nds_y, glm::mat4 projection, glm::mat4 view)
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

	glm::vec4 WorldEngine::GetPlaneIntersectPositionAndDis(glm::vec3 ray_dir, glm::vec3 cameraPosition, glm::mat4 model, glm::vec3 V1, glm::vec3 V2, glm::vec3 V3, glm::vec3 range_min, glm::vec3 range_max)//接收世界坐标
	{
		glm::vec3 dv1 = V2 - V1;
		glm::vec3 dv2 = V3 - V1;
		glm::mat3 coefficient = glm::mat3(-ray_dir, dv1, dv2);
		glm::vec3 value = cameraPosition - V1;
		glm::vec3 solution = glm::inverse(coefficient)*value;

		if (solution.x > 0)//防止反向射到
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

			if (pickLoc.x<range_min.x || pickLoc.x>range_max.x || pickLoc.y<range_min.y || pickLoc.y>range_max.y || pickLoc.z<range_min.z || pickLoc.z>range_max.z)
			{
				return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX); //超范围取消
			}
			else
			{
				//std::cout << "OR:" << dis(rLoc, V1) << " NR:" << dis(pickPoint, V1) << std::endl;
				return glm::vec4(pickPoint, solution.x);//传点和距离
			}
		}
		else
		{
			return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
		}
	}
	glm::vec4 WorldEngine::GetPlaneIntersectPositionAndDis(glm::vec3 ray_dir, glm::vec3 cameraPosition, glm::vec3 V1, glm::vec3 V2, glm::vec3 V3, glm::vec3 rLoc)
	{
		glm::vec3 dv1 = V2 - V1;
		glm::vec3 dv2 = V3 - V1;
		//glm::mat3 coefficient =  //错误！不能这么写
		//{
		//	0-ray_dir.x, dv1.x, dv2.x,
		//	0-ray_dir.y, dv1.y, dv2.y,
		//	0-ray_dir.z, dv1.z, dv2.z,
		//};
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
			if (dis(pickPoint, V1) > dis(rLoc, V1))
			{
				return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX); //超范围取消
			}
			else
			{
				//std::cout << "OR:" << dis(rLoc, V1) << " NR:" << dis(pickPoint, V1) << std::endl;
				return glm::vec4(pickPoint, solution.x);//传点和距离
			}
		}
		else
		{
			return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
		}

	}

	glm::vec4 WorldEngine::GetTreadIntersectPositionAndDisForWheelGroup(glm::vec3 ray_dir, glm::vec3 cameraPosition, glm::vec3 V1, glm::vec3 rWor, glm::mat4 model, float widthStart, float widthEnd)
	{

		float a = pow(ray_dir.x, 2) + pow(ray_dir.y, 2);
		float b = 2 * (cameraPosition.x*ray_dir.x + cameraPosition.y*ray_dir.y - V1.x*ray_dir.x - V1.y*ray_dir.y);
		float c = pow(cameraPosition.x, 2) + pow(cameraPosition.y, 2) + pow(V1.x, 2) + pow(V1.y, 2) - 2 * (V1.x*cameraPosition.x + V1.y*cameraPosition.y) - pow(dis(rWor, V1), 2);

		float J = pow(b, 2) - 4 * a*c;
		if (J < 0)
		{
			//std::cout << "no!" << std::endl;
			return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
		}
		else
		{
			float solution1 = (-b + sqrtf(J)) / (2.0f*a);
			float solution2 = (-b - sqrtf(J)) / (2.0f*a);
			if (solution1 <= 0 && solution2 <= 0)//反向射线
			{
				//std::cout << "no!" << std::endl;
				return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
			}
			else if (solution1 > 0 && solution2 <= 0)//内部射线
			{
				//std::cout << "haveN!" << std::endl;
				glm::vec3 pickPoint;
				pickPoint.x = cameraPosition.x + ray_dir.x*solution1;
				pickPoint.y = cameraPosition.y + ray_dir.y*solution1;
				pickPoint.z = cameraPosition.z + ray_dir.z*solution1;

				glm::vec4 pickWor = glm::vec4(glm::vec3(pickPoint), 1.0f);
				glm::vec3 pickLoc = glm::vec3(glm::inverse(model)*pickWor);
				if (pickLoc.y >= widthStart && pickLoc.y <= widthEnd)
				{

					//std::cout << "tx,ty:" << pickLoc.x << " " << pickLoc.y << std::endl;
					return glm::vec4(pickPoint, solution1);
				}
				else
				{
					return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
				}
			}
			else if (solution1 > 0 && solution2 > 0)//找到最近的点
			{
				//std::cout << "haveW!" << std::endl;
				glm::vec3 pickPoint;
				pickPoint.x = cameraPosition.x + ray_dir.x*solution2;
				pickPoint.y = cameraPosition.y + ray_dir.y*solution2;
				pickPoint.z = cameraPosition.z + ray_dir.z*solution2;

				glm::vec4 pickWor = glm::vec4(glm::vec3(pickPoint), 1.0f);  //拾取点是否在宽度范围内？
				glm::vec3 pickLoc = glm::vec3(glm::inverse(model)*pickWor);
				if (pickLoc.y >= widthStart && pickLoc.y <= widthEnd)
				{
					//std::cout << "tx,ty:" << pickLoc.x << " " << pickLoc.y << std::endl;
					return glm::vec4(pickPoint, solution2);
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

	void WorldEngine::SetActivateWheel(WheelSide side, int id/*, int outputId*/)//设定要激活轮子的位置,和轮子被点击时输出的ID
	{
		if (side == WheelSide::A)
		{
			id += train.get_marshalling()*8 / 2;//wheel count/2
			train.set_activate_wheel_id(id);
		}
		else if (side == WheelSide::B)
		{
			train.set_activate_wheel_id(id);
		}
	}
	//void WorldEngine::ActivateWheel(int id/*, int outputId*/)//设定要激活轮子的位置,和轮子被点击时输出的ID
	//{
	//	train.ActivateWheel(id, outputId);
	//}
	/*void WorldEngine::FreezeWheel(WheelSide side, int id)
	{
		if (side == WheelSide::A)
		{
			id += Train::kWheelCount / 2;
			train.FreezeWheel(id);
		}
		else if (side == WheelSide::B)
		{
			train.FreezeWheel(id);
		}
	}
	void WorldEngine::FreezeAllWheel()
	{
		train.FreezeAllWheel();
	}*/
}