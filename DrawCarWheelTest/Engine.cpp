#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
float dis(glm::vec3 p1,glm::vec3 p2)
{
	glm::vec3 d = p1 - p2;
	return sqrtf(pow(d.x, 2) + pow(d.y, 2) + pow(d.z, 2));
}
//void Engine::addMesh(const Mesh& mesh, glm::mat4 model)
//{
//	Object obj(mesh, model);
//	//std::cout << "initE1" << std::endl;
//	objects.push_back(obj);
//	//std::cout << "initE2" << std::endl;
//}
void Engine::SetWheelGroup(const Object& obj)
{
	this->wheelGroup.SetWheel(obj);
	this->wheelGroup.BuildModelAndActiveList();
}

void Engine::DrawWheelGroup(ShaderProgram &shader)
{
	this->wheelGroup.Draw(shader);
}

int Engine::RayPick_WheelGroup(double xpos, double ypos, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPosition, int scrWidth, int scrHeight)
{
	float nds_x = (2.0f * xpos) / scrWidth - 1.0f;//缩放到-1,0f-1.0f
	float ndx_y = 1.0f - (2.0f * ypos) / scrHeight;

	glm::vec3 ray_dir = this->GetRayDirection(nds_x, ndx_y, projection, view);
	int wheelSize = wheelGroup.Size();
	float minDis = FLT_MAX;
	float minI = -1;
	for (int i = 0; i != wheelSize; ++i)
	{
		float dis = this->IntersectWithCylinderForWheelGroup(wheelGroup.GetModel(i), WheelGroup::WheelWidth, WheelGroup::WheelRadius, ray_dir, cameraPosition);
		if (minDis > dis)
		{
			minDis = dis;
			minI = i;
		}
	}
	if (minI == -1)
	{
		return -1;
	}
	else
	{
		this->ActivateWheel(minI, 0);
		return wheelGroup.GetOutPutID(minI);
	}
	
}

void Engine::AddObject(const Object& obj)
{
	objects.push_back(obj);
}
void Engine::AddMesh(const Mesh& mesh,glm::mat4 model)
{
	Object obj(mesh, model);
	objects.push_back(obj);
}
int Engine::GetObjectsSize()
{
	return objects.size();
}
void Engine::DrawSingle(ShaderProgram &shader, int id)
{
	if (id >= this->objects.size()||id<0)
	{
		std::cout << "ERROR::ENGINE::DRAWSINGLE::ILLEGAL ID!" << std::endl;
	}
	//shader.setMat4("model",this->objects[id].model);
	this->objects[id].Draw(shader);
}
void Engine::Draw(ShaderProgram &shader)
{
	//移动
	//int size = objects.size();
	//for (int i = 0; i != size; ++i)
	//{
	//	shader.setMat4("model", objects[i].model);
	//	objects[i].mesh.Draw(shader);
	//}
	for (auto &i : objects)
	{
		shader.setMat4("model", i.GetModel());
		i.Draw(shader);
	}
	//std::cout << "Draw ok3" << std::endl;
}
float Engine::IntersectWithCylinderForWheelGroup(const glm::mat4& model, /*const glm::vec3& cylinderCenter, */float cylinderWidth, float cylinderRadius, const glm::vec3& ray_dir, const glm::vec3& cameraPosition)//仅限于Z轴方向上的轮子pick
{
	//针对object0
	
	glm::vec3 P1V1Loc(0.0f, 0.0f, 0.0f);  //平面1  v1要是圆点,原来的面是平躺着的，所有pick面也是躺着的,定制化，不能动
	glm::vec3 P1V2Loc(1.0f, 0.0f, 0.0f);
	glm::vec3 P1V3Loc(0.0f, 0.0f, 1.0f);
	glm::vec3 P1RLoc(cylinderRadius, 0.0f, 0.0f);

	glm::vec3 P2V1Loc(0.0f, 0.0f + cylinderWidth, 0.0f);  //平面2
	glm::vec3 P2V2Loc(1.0f, 0.0f + cylinderWidth, 0.0f);
	glm::vec3 P2V3Loc(0.0f, 0.0f + cylinderWidth, 1.0f);
	glm::vec3 P2RLoc(cylinderRadius, 0.0f + cylinderWidth , 0.0f);

	//glm::vec3 P1V1Loc(cylinderCenter.x, cylinderCenter.y, cylinderCenter.z - cylinderWidth / 2);  //平面1  v1要是圆点
	//glm::vec3 P1V2Loc(cylinderCenter.x + 1.0f, cylinderCenter.y, cylinderCenter.z - cylinderWidth / 2);
	//glm::vec3 P1V3Loc(cylinderCenter.x, cylinderCenter.y+1.0f, cylinderCenter.z - cylinderWidth / 2);
	//glm::vec3 P1RLoc(cylinderRadius, cylinderCenter.y, cylinderCenter.z - cylinderWidth / 2);

	//glm::vec3 P2V1Loc(cylinderCenter.x, cylinderCenter.y, cylinderCenter.z + cylinderWidth / 2);  //平面2
	//glm::vec3 P2V2Loc(cylinderCenter.x + 1.0f, cylinderCenter.y, cylinderCenter.z + cylinderWidth / 2);
	//glm::vec3 P2V3Loc(cylinderCenter.x, cylinderCenter.y + 1.0f, cylinderCenter.z + cylinderWidth / 2);
	//glm::vec3 P2RLoc(object.cylinderRadius, cylinderCenter.y, cylinderCenter.z + cylinderWidth / 2);

	//x^2+y^2=object.cylinderRadius

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
	if (minPD.w == FLT_MAX)
	{
		//std::cout << "no thing!" << std::endl;
		return FLT_MAX;
	}
	else//
	{
		return minPD.w;
	}
}
WheelPickMsg Engine::RayPick_Wheel(double xpos, double ypos, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPosition, int scrWidth, int scrHeight)
{
	float nds_x = (2.0f * xpos) / scrWidth - 1.0f;//缩放到-1,0f-1.0f
	float ndx_y = 1.0f - (2.0f * ypos) / scrHeight;
	//float z = -1.0f;//投影到远裁剪平面
	glm::vec3 ray_dir = this->GetRayDirection(nds_x, ndx_y, projection, view);
	
	//针对object0
	auto &object = objects[0];

	glm::vec3 P1V1Loc(0.0f, 0.0f, 0.0f);  //平面1  v1要是圆点
	glm::vec3 P1V2Loc(1.0f, 0.0f, 0.0f);
	glm::vec3 P1V3Loc(0.0f, 1.0f, 0.0f);
	glm::vec3 P1RLoc(Wheel::STANDARDTREADRADIUS, 0.0f, 0.0f);

	glm::vec3 P2V1Loc(0.0f, 0.0f, Wheel::STANDARDWIDTH);  //平面2
	glm::vec3 P2V2Loc(1.0f, 0.0f, Wheel::STANDARDWIDTH);
	glm::vec3 P2V3Loc(0.0f, 1.0f, Wheel::STANDARDWIDTH);
	glm::vec3 P2RLoc(Wheel::STANDARDTREADRADIUS, 0.0f, Wheel::STANDARDWIDTH);

	//x^2+y^2=object.cylinderRadius

	glm::vec3 P1V1Wor = glm::vec3(object.GetModel()*glm::vec4(P1V1Loc, 1.0f));
	glm::vec3 P1V2Wor = glm::vec3(object.GetModel()*glm::vec4(P1V2Loc, 1.0f));
	glm::vec3 P1V3Wor = glm::vec3(object.GetModel()*glm::vec4(P1V3Loc, 1.0f));
	glm::vec3 P1RWor= glm::vec3(object.GetModel()*glm::vec4(P1RLoc, 1.0f));

	glm::vec4 P1PD=this->GetPlaneIntersectPositionAndDis(ray_dir, cameraPosition, P1V1Wor, P1V2Wor, P1V3Wor, P1RWor);

	glm::vec3 P2V1Wor = glm::vec3(object.GetModel()*glm::vec4(P2V1Loc, 1.0f));
	glm::vec3 P2V2Wor = glm::vec3(object.GetModel()*glm::vec4(P2V2Loc, 1.0f));
	glm::vec3 P2V3Wor = glm::vec3(object.GetModel()*glm::vec4(P2V3Loc, 1.0f));
	glm::vec3 P2RWor = glm::vec3(object.GetModel()*glm::vec4(P2RLoc, 1.0f));

	glm::vec4 P2PD = this->GetPlaneIntersectPositionAndDis(ray_dir, cameraPosition, P2V1Wor, P2V2Wor, P2V3Wor, P2RWor);

	glm::vec4 TPD = this->GetTreadIntersectPositionAndDisForWheel(ray_dir, cameraPosition, P1V1Wor, P1RWor,object.GetModel(), 0, Wheel::STANDARDWIDTH);

	//std::cout << "msg: " << P1PD.w<<" "<<P2PD.w<<" "<<TPD.w<<std::endl;
	glm::vec4 minPPD = P1PD.w > P2PD.w ? P2PD : P1PD;
	glm::vec4 minPD = TPD.w > minPPD.w ? minPPD : TPD;
	if (minPD.w == FLT_MAX)
	{
		//std::cout << "no thing!" << std::endl;
		return WheelPickMsg(-1,-1);
	}
	else//
	{
		glm::vec4 pickWor = glm::vec4(glm::vec3(minPD),1.0f);
		glm::vec3 pickLoc = glm::vec3(glm::inverse(object.GetModel())*pickWor);

		float x = pickLoc.x;
		float y = pickLoc.y;
		float angle = atan2(y, x);
		angle = angle < 0 ? (angle + 2 * Wheel::PIR) : angle;//弧度
		//float r = sqrt(pow(x, 2) + pow(y, 2));
		if (minPD == TPD)
		{
			float z = pickLoc.z;
			return WheelPickMsg(angle,z);
		}
		else
		{
			return WheelPickMsg(angle, -1);
		}
		
		//angle = 360*(angle / (2 * PIR));//角度a
		//std::cout << "cameraPosition" << cameraPosition.x << " " << cameraPosition.y << " " << cameraPosition.z << std::endl;
		//std::cout <<"x,y:"<< x <<" "<< y <<std::endl;
		//std::cout << "angle: " << angle << std::endl;
		//std::cout << std::endl;	
	}

	//std::cout << ray_dir.x << " " << ray_dir.y << " " << ray_dir.z << std::endl;
	//
	//auto &object = objects[0];
	//auto &indice = object.mesh.indices;
	//auto &vertice = object.mesh.vertices;
	//
	//glm::vec3 minV1Loc, minV2Loc, minV3Loc;
	//float minDis=-1;
	//for (int i = 0; i != indice.size(); i += 3)
	//{
	//	glm::vec3 V1Loc = vertice[indice[i]].Position;
	//	glm::vec3 V2Loc = vertice[indice[i + 1]].Position;
	//	glm::vec3 V3Loc = vertice[indice[i + 2]].Position;
	//
	//	glm::vec3 V1= glm::vec3(object.model*glm::vec4(V1Loc,1.0f));
	//	glm::vec3 V2 = glm::vec3(object.model*glm::vec4(V2Loc, 1.0f));
	//	glm::vec3 V3 = glm::vec3(object.model*glm::vec4(V3Loc, 1.0f));
	//	float dis = this->GetIntersectDis(ray_dir, cameraPosition, V1, V2, V3);
	//	if(dis&&(minDis == -1 || minDis > dis))
	//	{
	//		minDis = dis;
	//		minV1Loc = V1Loc;
	//		minV2Loc = V2Loc;
	//		minV3Loc = V3Loc;
	//	}
	//}
	//
	//if (minDis == -1)
	//{
	//	//std::cout << "no thing!" << std::endl;
	//}
	//else
	//{
	//	int x = minV1Loc.x;
	//	int y = minV1Loc.y;
	//
	//	float r = sqrtf(pow(x, 2) + pow(y, 2));
	//	float angle = atan2(y, x);
	//	angle = angle < 0 ? (angle + 2 * PIR) : angle;//弧度
	//	angle = 360 * angle / 2 * PIR;//角度a
	//
	//	//std::cout << x<<" "<<y<<" dis: " << minDis <<" angle: "<< angle <<" r: "<<r<< std::endl;
	//	
	//}

}
glm::vec3 Engine::GetRayDirection(double nds_x, double nds_y, glm::mat4 projection, glm::mat4 view)
{
	glm::vec3 ray_nds = glm::vec3(nds_x, nds_y, -1.0f);
	glm::vec4 ray_clip = glm::vec4(ray_nds, 1.0f);//裁剪坐标

												  //camera
	glm::vec4 ray_camera = glm::inverse(projection)*ray_clip;
	ray_camera= glm::vec4(ray_camera.x, ray_camera.y, -1.0, 0.0);//没有裁剪(标准化)的摄像机坐标，所以把射线目的坐标z轴设为-1
	//world
	glm::vec3 ray_world = glm::normalize(glm::vec3(glm::inverse(view)*ray_camera));//不需要cameraPosition，view中就包含cameraPosition的信息，相当于直接转化射线

	glm::vec3 ray_dir = glm::normalize(glm::vec3(ray_world));
	
	//std::cout << "ray_world:" << ray_world.x << " " << ray_world.y << " " << ray_world.z << std::endl;


	return ray_dir;
}


glm::vec4 Engine::GetPlaneIntersectPositionAndDis(glm::vec3 ray_dir,glm::vec3 cameraPosition,glm::vec3 V1, glm::vec3 V2, glm::vec3 V3, glm::vec3 rLoc)
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
	if (isinf(solution.x))
	{
		return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
	}
	else if(solution.x>0)//防止反向射到
	{
		float u = solution.y;
		float v = solution.z;
		//if (solution.x > 0.9)
		//{
			//std::cout <<"len:"<< solution.x <<" u:"<<u <<" v:"<< v << std::endl;
		//}
		glm::vec3 pickPoint = V1 + u*dv1 + v*dv2;
		if (dis(pickPoint, V1) > dis(rLoc,V1))
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

	//float u = solution.y;
	//float v = solution.z;
	//float angle = atan2(v, u);
	//angle = angle < 0 ? (angle + 2 * PIR) : angle;//弧度
	//angle = 360*(angle / (2 * PIR));//角度a
	//std::cout << "cameraPosition" << cameraPosition.x << " " << cameraPosition.y << " " << cameraPosition.z << std::endl;
	//std::cout <<"l,x,y:"<< solution.x <<" "<< u << " " << v << std::endl;
	//std::cout << "angle: " << angle << std::endl;
	//std::cout << std::endl;
	//else 
	//{
	//	float u = solution.y;
	//	float v = solution.z;
	//	if (u >= 0 && v >= 0 && u + v - 1 <=0)
	//	{
	//		return solution.x;
	//	}
	//	else
	//	{
	//		return -1;
	//	}
	//}
}

glm::vec4 Engine::GetTreadIntersectPositionAndDisForWheel(glm::vec3 ray_dir, glm::vec3 cameraPosition, glm::vec3 V1, glm::vec3 rWor,glm::mat4 model,float widthStart,float widthEnd)
{
	//float x=cameraPosition.x+ray_dir.x*len;
	//float y=cameraPosition.y+ray_dir.y*len;
	//(x-V1.x)^2+(y-V1.y)^2=dis(rLoc,V1)^2
	float a = pow(ray_dir.x, 2) + pow(ray_dir.y, 2);
	float b = 2 * (cameraPosition.x*ray_dir.x + cameraPosition.y*ray_dir.y - V1.x*ray_dir.x - V1.y*ray_dir.y);
	float c = pow(cameraPosition.x, 2) + pow(cameraPosition.y, 2) +pow(V1.x,2)+pow(V1.y,2)- 2*(V1.x*cameraPosition.x + V1.y*cameraPosition.y) - pow(dis(rWor, V1), 2);

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
			if (pickLoc.z >= widthStart && pickLoc.z <= widthEnd)
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
			if (pickLoc.z >= widthStart && pickLoc.z <= widthEnd)
			{
				//std::cout << "tx,ty:" << pickLoc.x << " " << pickLoc.y << std::endl;
				return glm::vec4(pickPoint, solution2);
			}
			else
			{
				return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
			}

		}
	}
}

glm::vec4 Engine::GetTreadIntersectPositionAndDisForWheelGroup(glm::vec3 ray_dir, glm::vec3 cameraPosition, glm::vec3 V1, glm::vec3 rWor, glm::mat4 model, float widthStart, float widthEnd)
{
	//float x=cameraPosition.x+ray_dir.x*len;
	//float y=cameraPosition.y+ray_dir.y*len;
	//(x-V1.x)^2+(y-V1.y)^2=dis(rLoc,V1)^2
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
	}
}

void Engine::ActivateWheel(WheelSide side, int id, int outputId)//设定要激活轮子的位置,和轮子被点击时输出的ID
{
	if (side == WheelSide::A)
	{
		id += WheelGroup::WheelCount / 2;
		wheelGroup.ActivateWheel(id, outputId);
	}
	else if(side == WheelSide::B)
	{
		wheelGroup.ActivateWheel(id, outputId);
	}
}
void Engine::ActivateWheel(int id, int outputId)//设定要激活轮子的位置,和轮子被点击时输出的ID
{
	wheelGroup.ActivateWheel(id, outputId);
}
void Engine::FreezeWheel(WheelSide side, int id)
{
	if (side == WheelSide::A)
	{
		id += WheelGroup::WheelCount / 2;
		wheelGroup.FreezeWheel(id);
	}
	else if (side == WheelSide::B)
	{
		wheelGroup.FreezeWheel(id);
	}
}
void Engine::FreezeAllWheel()
{
	wheelGroup.FreezeAllWheel();
}