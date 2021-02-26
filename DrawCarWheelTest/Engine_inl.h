#pragma once
namespace Engine
{
	inline float dis(glm::vec3 p1, glm::vec3 p2)
	{
		glm::vec3 d = p1 - p2;
		return sqrtf(pow(d.x, 2) + pow(d.y, 2) + pow(d.z, 2));
	}
}