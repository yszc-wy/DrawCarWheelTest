#ifndef COORDINATEAXIS_H
#define COORDINATEAXIS_H
#include <vector>

#include <glm\glm.hpp>

#include "MeshObject.h"
#include "ShaderProgram.h"
class CoordinateAxis{
public:
    static const glm::vec3 Red;
    static const glm::vec3 Green;
    static const glm::vec3 Blue;
    static const glm::vec3 Yellow;
public:
    CoordinateAxis():arrow(),line(){}
    void SetArrow(const Object::MeshObject& obj);
    void SetLine(const Object::MeshObject& obj);
    void BuildModelAndColor();
    void Draw(ShaderProgram& shader);
private:
	Object::MeshObject arrow;
	Object::MeshObject line;
    std::vector<glm::mat4> arrowsModel;
    std::vector<glm::vec3> arrowsColor;
    std::vector<glm::mat4> linesModel;
    std::vector<glm::vec3> linesColor;
};

#endif // COORDINATEAXIS_H
