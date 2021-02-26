#include "CoordinateAxis.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

const glm::vec3 CoordinateAxis::Red=glm::vec3(1.0f,0.0f,0.0f);
const glm::vec3 CoordinateAxis::Green=glm::vec3(0.0f,1.0f,0.0f);
const glm::vec3 CoordinateAxis::Blue=glm::vec3(0.0f,0.0f,1.0f);
const glm::vec3 CoordinateAxis::Yellow=glm::vec3(1.0f,1.0f,0.0f);
void CoordinateAxis::SetArrow(const Object::MeshObject& obj)
{
    this->arrow=obj;
}
void CoordinateAxis::SetLine(const Object::MeshObject& obj)
{
    this->line=obj;
}
void CoordinateAxis::BuildModelAndColor()//九线
{
    glm::mat4 scaleArrow(1.0f);
    scaleArrow = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.3f, 0.5f));

    glm::mat4 translateArrow(1.0f);
    translateArrow=glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, -0.1f));

    glm::mat4 scaleArrowLine = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 3.0f, 5.0f));
    glm::mat4 scaleLine = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 2.5f, 5.0f));

    glm::mat4 translateLine=glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, -0.01f));

    glm::mat4 rotate(1.0f);

    //竖轴
    rotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    this->arrowsModel.push_back(rotate*scaleArrow*translateArrow);
    this->arrowsColor.push_back(Blue);

    //横纵
    //y轴
    rotate = glm::mat4(1.0f);
    this->arrowsModel.push_back(rotate*scaleArrow*translateArrow);
    this->arrowsColor.push_back(Green);

    rotate = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f))*rotate;
    this->linesModel.push_back(rotate*scaleArrowLine*translateLine);
    this->linesColor.push_back(Green);

    //x轴
    rotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f))*rotate;
    this->arrowsModel.push_back(rotate*scaleArrow*translateArrow);
    this->arrowsColor.push_back(Red);

    rotate = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f))*rotate;
    this->linesModel.push_back(rotate*scaleArrowLine*translateLine);
    this->linesColor.push_back(Red);


    //45度
    rotate = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    this->linesModel.push_back(rotate*scaleLine*translateLine);
    this->linesColor.push_back(Yellow);

    rotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f))*rotate;
    this->linesModel.push_back(rotate*scaleLine*translateLine);
    this->linesColor.push_back(Yellow);

    rotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f))*rotate;
    this->linesModel.push_back(rotate*scaleLine*translateLine);
    this->linesColor.push_back(Yellow);

    rotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f))*rotate;
    this->linesModel.push_back(rotate*scaleLine*translateLine);
    this->linesColor.push_back(Yellow);

}
void CoordinateAxis::Draw(ShaderProgram& shader)
{
    if (this->arrowsModel.size() == 0||this->arrow.MeshSize()==0)
    {
            std::cout << "ERROR::CORRDINATEAXIS::TRY TO DRAW AXIS WHICH Arrow OBJECT IS NOT INIT!" << std::endl;
    }
    else
    {
        int size=this->arrowsModel.size();
        for(int i=0;i!=size;++i)
        {
            this->arrow.SetModel(this->arrowsModel[i]);
            this->arrow.SetColor(this->arrowsColor[i]);
            this->arrow.Draw(shader);
        }
    }

    if (this->linesModel.size() == 0||this->line.MeshSize()==0)
    {
            std::cout << "ERROR::CORRDINATEAXIS::TRY TO DRAW AXIS WHICH Line OBJECT IS NOT INIT!" << std::endl;
    }
    else
    {
        int size=this->linesModel.size();
        for(int i=0;i!=size;++i)
        {
            this->line.SetModel(this->linesModel[i]);
            this->line.SetColor(this->linesColor[i]);
            this->line.Draw(shader);
        }
    }
}
