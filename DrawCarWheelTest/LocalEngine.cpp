#include "LocalEngine.h"

#include <iostream>
#include <string>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine_inl.h"
#include "Wheel.h"
namespace Engine {
	//void LocalEngine::AddWheel(const Object& flangeTread,const Object& floatPlant)
	//{
	//	wheelFlangeTreads.push_back(flangeTread);
	//	wheelFloatPlants.push_back(floatPlant);
	//}
	//void LocalEngine::AddWheel(const Mesh& flangeTreadMesh, glm::mat4 flangeTreadModel, glm::vec3 flangeTreadColor,const Mesh& floatPlantMesh, glm::mat4 floatPlantModel, glm::vec3 floatPlantColor)
	//{
	//	Object flangeTread(flangeTreadMesh, flangeTreadModel, flangeTreadColor);
	//	wheelFlangeTreads.push_back(flangeTread);

	//	Object floatPlant(floatPlantMesh, floatPlantModel, floatPlantColor);
	//	wheelFloatPlants.push_back(floatPlant);
	//}
	void LocalEngine::AddPolarCoordinate(const Object::CoordinateObject& obj)
	{
		this->polar_coordinate.push_back(obj);
	}
	void LocalEngine::AddCartesianCoordinate(const Object::CoordinateObject& obj)
	{
		this->cartesian_coordinate.push_back(obj);
	}
	void LocalEngine::DrawPolarCoordinates(ShaderProgram &line_shader, ShaderProgram &font_shader)
	{
		for (auto &coordinate : this->polar_coordinate)
		{
			coordinate.Draw(line_shader, font_shader);
		}
	}
	void LocalEngine::DrawCartesianCoordinates(ShaderProgram &line_shader, ShaderProgram &font_shader)
	{
		for (auto &coordinate : this->cartesian_coordinate)
		{
			coordinate.Draw(line_shader, font_shader);
		}
	}
	void LocalEngine::UpdateTextDirection(const glm::vec3& camera_position, const glm::vec3& camera_up, const glm::mat4& projection, const glm::mat4& view)
	{
		for (auto &coordinate : this->polar_coordinate)
		{
			coordinate.UpdateTextDirection(camera_position, camera_up, projection, view);
		}
		for (auto &coordinate : this->cartesian_coordinate)
		{
			coordinate.UpdateTextDirection(camera_position, camera_up, projection, view);
		}
	}

	void LocalEngine::AddWheelObject(const WheelObject& wheelObj)
	{
		if (this->wheelFloatPlants.find(wheelObj.get_float_plant_path()) == this->wheelFloatPlants.end())
		{
			std::cout << "Warning::AddWheelObject::There is not fit float plant to this wheelObj. Be sure to add floatPlant after this action." << std::endl;
		}
		if (this->wheelObjects.size() != 0)
		{
			this->wheelObjects[0].DeleteWheelObject();
			this->wheelObjects.clear();
		}
		this->wheelObjects.push_back(wheelObj);
	}
	void LocalEngine::AddWheelFloatPlant(const std::string& floatPlantPath, const Object::MeshObject& floatPlantObj)
	{
		if (this->wheelFloatPlants.find(floatPlantPath) == this->wheelFloatPlants.end())
		{
			this->wheelFloatPlants.insert({ floatPlantPath,floatPlantObj });
		}
		else
		{
			std::cout << "ERROR::AddWheelFloatPlant::Same float plant can not be added" << std::endl;
		}
	}
	void LocalEngine::SetCurrentWheelId(int current_wheel_id)
	{
		if (current_wheel_id < 0 || current_wheel_id >= this->wheelObjects.size())
		{
			std::cout << "LocalEngine::SetCurrentWheelId::illegal current id" << std::endl;
		}
		this->currentWheelId = current_wheel_id;
	}
	void LocalEngine::UpdateFloatPlant(const std::string& float_plant_file_path, const Object::MeshObject& float_plant_obj,
		/*const std::map<std::string, std::vector<glm::vec2>>& floatPlants,*/
		const std::vector<glm::vec2>& float_plant,
		const std::vector<Wheel::WheelFlangeTread>& wheel_flange_treads)
	{
		//std::cout << "LocalEngine::UpdateFloatPlant::OK?" << std::endl;
		if (this->wheelFloatPlants.find(float_plant_file_path) == this->wheelFloatPlants.end())
		{
			this->wheelFloatPlants.insert({ float_plant_file_path,float_plant_obj });
		}
		else
		{
			//切换浮板
			this->wheelFloatPlants[float_plant_file_path].DeleteMeshs();
			this->wheelFloatPlants[float_plant_file_path] = float_plant_obj;

			auto wheel_obj_size = this->wheelObjects.size();
			for (int i=0;i!= wheel_obj_size;++i)
			{
				if (this->wheelObjects[i].get_float_plant_path() != float_plant_file_path)
				{
					continue;
				}

				glm::mat4 model(1.0f);
				model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));//缩放
				glm::vec3 color(150.0f / 255.0f, 150.0f / 255.0f, 150.0f / 255.0f);

				//使用这个浮板的重新生成connection
				
				OpenglObj::OpenglPointMsg wheelConnectionMsg = Wheel::LoadConnectionPoint(wheel_flange_treads[i].get_flange_tread(), float_plant);
				
				this->wheelObjects[i].set_wheel_connection(Object::MeshObject(OpenglObj::Mesh(wheelConnectionMsg.GetVertices(), wheelConnectionMsg.GetIndices()), model, color));
			}
		}
	}

	void LocalEngine::DrawWheelFloatPlantAndConnection(ShaderProgram &shader, int id)
	{
		if (id < 0 || id >= this->wheelObjects.size())
		{
			std::cout << "Error::LocalEngine::DrawWheelFloatPlantAndConnection::illgel id" << std::endl;
		}
		else
		{
			this->currentWheelId = id;
			this->wheelObjects[id].get_wheel_connection().Draw(shader);

			if (this->wheelFloatPlants.find(this->wheelObjects[id].get_float_plant_path()) != this->wheelFloatPlants.end())
			{
				//shader.use();
				this->wheelFloatPlants[this->wheelObjects[id].get_float_plant_path()].Draw(shader);
			}
			else
			{
				std::cout << "Error::LocalEngine::DrawWheelFloatPlantAndConnection::Can not find float plant path to draw." << std::endl;
			}
		}
	}
	void LocalEngine::DrawWheelFlangeTread(ShaderProgram &shader, int id)
	{
		if (id < 0 || id >= this->wheelObjects.size())
		{
			std::cout << "Error::LocalEngine::DrawWheelFlangeTread::illgel id" << std::endl;
		}
		else
		{
			this->currentWheelId = id;
			this->wheelObjects[id].get_wheel_flange_tread().Draw(shader);
		}
	}
	void LocalEngine::UpdateWheelFlangeTreadModel(glm::mat4 model, int id)
	{
		if (id < 0 || id >= this->wheelObjects.size())
		{
			std::cout << "Error::LocalEngine::UpdateWheelModel::illgel id" << std::endl;
		}
		else
		{
			this->wheelObjects[id].set_wheel_flange_tread_model(model);
		}
	}
	//void LocalEngine::AddWheel(const Mesh& mesh, glm::mat4 model, glm::vec3 color)
	//{
	//	Object obj(mesh, model, color);
	//	wheels.push_back(obj);
	//}
	//void LocalEngine::AddWheel(const Object& obj)
	//{
	//	wheels.push_back(obj);
	//}

	void LocalEngine::SetCoordinateAxis(const Object::MeshObject& arrowObj, const Object::MeshObject& lineObj)
	{
		this->coordinateAxis.SetArrow(arrowObj);
		this->coordinateAxis.SetLine(lineObj);
		this->coordinateAxis.BuildModelAndColor();
	}
	void LocalEngine::DrawCoordinateAxis(ShaderProgram &shader)
	{
		//shader.setInt("axis", true);
		this->coordinateAxis.Draw(shader);
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
	int LocalEngine::GetWheelsSize()
	{
		//return wheelFlangeTreads.size();
		return wheelObjects.size();
	}
	//void LocalEngine::SetCurrentId(int id)
	//{
	//	this->currentWheelId = id;
	//}
	//void LocalEngine::DrawWheelFlangeTread(ShaderProgram &shader, int id, const std::vector<glm::vec2>& averRadius)
	//{
	//	if (id >= this->wheels.size() || id < 0)
	//	{
	//		std::cout << "ERROR::ENGINE::DRAWSINGLE::ILLEGAL ID!" << std::endl;
	//	}
	//	else//shader.setMat4("model",this->wheels[id].model);
	//	{
	//		shader.setInt("axis", false);
	//		int size = averRadius.size();
	//		for (int i = 0; i != size; ++i)
	//		{
	//			std::string name = "averRadius[" + std::to_string(i) + "]";
	//			//glm::vec2 vec = averRadius[i];
	//			shader.setVec2(name, averRadius[i]);
	//		}
	//		shader.setInt("averSize", size);
	//		this->wheels[id].Draw(shader);
	//	}
	//}
	//void LocalEngine::DrawObjects(ShaderProgram &shader)
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
	void LocalEngine::SwitchFloatPlant(const std::string& file_path, const Object::MeshObject& connectionObj)
	{
		if (this->wheelFloatPlants.find(file_path) == this->wheelFloatPlants.end())
		{
			std::cout << "Error::LocalEngine::SwitchFloatPlant::illegal file_path" << std::endl;
			return;
		}
		if (this->currentWheelId < 0 || this->currentWheelId >= this->wheelObjects.size())
		{
			std::cout << "Error::LocalEngine::SwitchFloatPlant::illegal current id" << std::endl;
			return;
		}
		if (this->wheelObjects[currentWheelId].get_float_plant_path() == file_path)
		{
			std::cout << "Warning::LocalEngine::SwitchFloatPlant::same float plant" << std::endl;
			return;
		}

		this->wheelObjects[currentWheelId].set_float_plant_path(file_path);
		this->wheelObjects[currentWheelId].set_wheel_connection(connectionObj);
	}
	std::string LocalEngine::GetCurrentFloatPlantPath() const
	{
		if (this->currentWheelId < 0 || this->currentWheelId >= this->wheelObjects.size())
		{
			std::cout << "Error::LocalEngine::GetCurrentFloatPlantPath::illegal current id" << std::endl;
			return "";
		}
		return this->wheelObjects[currentWheelId].get_float_plant_path();
	}
	PickMsg LocalEngine::RayPick_Wheel(double xpos, double ypos, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPosition, int scrWidth, int scrHeight)
	{
		float nds_x = (2.0f * xpos) / scrWidth - 1.0f;//缩放到-1,0f-1.0f
		float ndx_y = 1.0f - (2.0f * ypos) / scrHeight;
		//float z = -1.0f;//投影到远裁剪平面
		//std::cout << "right0?" << std::endl;
		glm::vec3 ray_dir = this->GetRayDirection(nds_x, ndx_y, projection, view);
		//std::cout << "right0.1?" << std::endl;
		//针对object0
		glm::mat4 objectModel;
		if (this->currentWheelId < 0 || this->currentWheelId >= this->wheelObjects.size())
		{
			std::cout << "Error::LocalEngine::RayPick_Wheel::illgel CurrentWheelId." << std::endl;
			return PickMsg(-1, -1);
		}
		else
			objectModel = wheelObjects[this->currentWheelId].get_wheel_flange_tread().GetModel();

		glm::vec3 P1V1Loc(0.0f, 0.0f, 0.0f);  //平面1  v1要是圆点
		glm::vec3 P1V2Loc(1.0f, 0.0f, 0.0f);
		glm::vec3 P1V3Loc(0.0f, 1.0f, 0.0f);
		glm::vec3 P1RLoc(Wheel::WheelFlangeTread::kStandardTreadRadius, 0.0f, 0.0f);

		glm::vec3 P2V1Loc(0.0f, 0.0f, Wheel::WheelFlangeTread::kStandardFlangeTreadWidth);  //平面2
		glm::vec3 P2V2Loc(1.0f, 0.0f, Wheel::WheelFlangeTread::kStandardFlangeTreadWidth);
		glm::vec3 P2V3Loc(0.0f, 1.0f, Wheel::WheelFlangeTread::kStandardFlangeTreadWidth);
		glm::vec3 P2RLoc(Wheel::WheelFlangeTread::kStandardTreadRadius, 0.0f, Wheel::WheelFlangeTread::kStandardFlangeTreadWidth);

		//x^2+y^2=object.cylinderRadius

		glm::vec3 P1V1Wor = glm::vec3(objectModel*glm::vec4(P1V1Loc, 1.0f));
		glm::vec3 P1V2Wor = glm::vec3(objectModel*glm::vec4(P1V2Loc, 1.0f));
		glm::vec3 P1V3Wor = glm::vec3(objectModel*glm::vec4(P1V3Loc, 1.0f));
		glm::vec3 P1RWor = glm::vec3(objectModel*glm::vec4(P1RLoc, 1.0f));

		//std::cout << "right1?" << std::endl;
		glm::vec4 P1PD = this->GetPlaneIntersectPositionAndDis(ray_dir, cameraPosition, P1V1Wor, P1V2Wor, P1V3Wor, P1RWor);

		glm::vec3 P2V1Wor = glm::vec3(objectModel*glm::vec4(P2V1Loc, 1.0f));
		glm::vec3 P2V2Wor = glm::vec3(objectModel*glm::vec4(P2V2Loc, 1.0f));
		glm::vec3 P2V3Wor = glm::vec3(objectModel*glm::vec4(P2V3Loc, 1.0f));
		glm::vec3 P2RWor = glm::vec3(objectModel*glm::vec4(P2RLoc, 1.0f));

		//std::cout << "right2?" << std::endl;
		glm::vec4 P2PD = this->GetPlaneIntersectPositionAndDis(ray_dir, cameraPosition, P2V1Wor, P2V2Wor, P2V3Wor, P2RWor);

		//float start = 0;
		//float end = 0;
		//if (this->currentWheelId != -1)
		//{
		//	start=this->ranges[currentWheelId].GetStart();
		//	end = this->ranges[currentWheelId].GetEnd();
		//}
		//std::cout << "right3?" << std::endl;
		glm::vec4 TPD = this->GetTreadIntersectPositionAndDisForWheel(ray_dir, cameraPosition, P1V1Wor, P1RWor, objectModel, 0, Wheel::WheelFlangeTread::kStandardFlangeTreadWidth);

		//std::cout << "msg: " << P1PD.w<<" "<<P2PD.w<<" "<<TPD.w<<std::endl;
		glm::vec4 minPPD = P1PD.w > P2PD.w ? P2PD : P1PD;
		glm::vec4 minPD = TPD.w > minPPD.w ? minPPD : TPD;
		if (minPD.w == FLT_MAX)
		{
			//std::cout << "no thing!" << std::endl;
			return PickMsg(-1, -1);
		}
		else//
		{
			glm::vec4 pickWor = glm::vec4(glm::vec3(minPD), 1.0f);
			glm::vec3 pickLoc = glm::vec3(glm::inverse(objectModel)*pickWor);

			float x = pickLoc.x;
			float y = pickLoc.y;
			float radian = atan2(y, x);
			radian = radian < 0 ? (radian + 2 * Wheel::WheelFlangeTread::kPIR) : radian;//弧度
																 //float r = sqrt(pow(x, 2) + pow(y, 2));
			if (minPD == TPD)
			{
				float z = pickLoc.z;
				return PickMsg(radian, z);
			}
			else
			{
				return PickMsg(radian, -1);
			}

			//angle = 360*(radian / (2 * PIR));//角度a
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
		//	float radian = atan2(y, x);
		//	radian = radian < 0 ? (radian + 2 * PIR) : radian;//弧度
		//	angle = 360 * radian / (2 * PIR);//角度a
		//
		//	//std::cout << x<<" "<<y<<" dis: " << minDis <<" angle: "<< angle <<" r: "<<r<< std::endl;
		//	
		//}

	}

	glm::vec3 LocalEngine::GetRayDirection(double nds_x, double nds_y, glm::mat4 projection, glm::mat4 view)
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

	glm::vec4 LocalEngine::GetPlaneIntersectPositionAndDis(glm::vec3 ray_dir, glm::vec3 cameraPosition, glm::vec3 V1, glm::vec3 V2, glm::vec3 V3, glm::vec3 rLoc)
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

		//float u = solution.y;
		//float v = solution.z;
		//float radian = atan2(v, u);
		//radian = radian < 0 ? (radian + 2 * PIR) : radian;//弧度
		//angle = 360*(radian / (2 * PIR));//角度a
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

	glm::vec4 LocalEngine::GetTreadIntersectPositionAndDisForWheel(glm::vec3 ray_dir, glm::vec3 cameraPosition, glm::vec3 V1, glm::vec3 rWor, glm::mat4 model, float widthStart, float widthEnd)
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
			else
			{
				return glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
			}
		}
	}
}