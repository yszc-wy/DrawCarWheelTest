#pragma once
#include "MeshObject.h"
class WheelObject
{
public:
	WheelObject(const Object::MeshObject& flangeTread, const Object::MeshObject& connection,const std::string& floatPlantPath)
		:wheel_flange_tread(flangeTread),
		wheel_connection(connection),
		float_plant_path(floatPlantPath)
	{}
	const Object::MeshObject& get_wheel_flange_tread() const
	{
		return wheel_flange_tread;
	}
	const Object::MeshObject& get_wheel_connection() const
	{
		return wheel_connection;
	}
	const std::string& get_float_plant_path() const
	{
		return float_plant_path;
	}
	void set_wheel_flange_tread_model(const glm::mat4 & model)
	{
		this->wheel_flange_tread.SetModel(model);
	}
	void set_float_plant_path(const std::string& file_path)
	{
		this->float_plant_path = file_path;
	}
	void set_wheel_connection(const Object::MeshObject& wheel_connection)
	{
		this->wheel_connection.DeleteMeshs();
		this->wheel_connection = wheel_connection;
	}
	void DeleteWheelObject()
	{
		wheel_flange_tread.DeleteMeshs();
		wheel_connection.DeleteMeshs();
	}
private:
	Object::MeshObject wheel_flange_tread;
	Object::MeshObject wheel_connection;
	std::string float_plant_path;
};