#version 330 core
out vec4 FragColor;

in vec3 Normals;  
in vec3 FragPos;  
in vec3 FragPosLoc;
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float pickRadian;
uniform float pickWidth;
uniform vec2 averRadius[400];
uniform int averSize;
uniform bool axis;
uniform bool redMark;
float GetR(float x,float y)
{
  return sqrt(pow(x,2.0)+pow(y,2.0));
}
int FindID(float width)
{ 
  int mid, left = 0, right = averSize - 1;//闭区间[0, n - 1]
	while (left < right)
	{
		mid = left + ((right + 1 - left) >> 1);//向上取整
		if (averRadius[mid].x < width) left = mid;
		else right = mid - 1;
	}

	if (averRadius[left].x <= width&&width<=averRadius[averSize - 1].x)
	{
		return left;
	}
	else
	{
		return -1;
	}
}
void main()
{
    float radian=atan(FragPosLoc.y,FragPosLoc.x);
    radian = radian < 0 ? (radian + 2 * 3.1415926) : radian;
    
    float width=FragPosLoc.z;
    float r=GetR(FragPosLoc.y,FragPosLoc.x);
    int id=FindID(width);
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normals);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result;
    
      if(axis)
      {
        result = (ambient + diffuse + specular) * objectColor;
      }
      else if(abs(radian-pickRadian)<0.005&&pickRadian>=0)
      {
        result=(ambient + diffuse + specular)*vec3(0.0,0.0,1.0);
      }
      else if(abs(width-pickWidth)<0.5&&pickWidth>=0)
      {
        result=(ambient + diffuse + specular)*vec3(0.0,0.0,0.0);
      }
      else if(redMark&&id!=-1&&r>380.0&&r<averRadius[id].y)//id!=-1&&r<averRadius[id].y&&r>380.0&&width>=50.0&&width<=78.8
      {
        result=(ambient + diffuse + specular)*vec3(1.0,0.0,0.0);
      }
      else
      {
        result = (ambient + diffuse + specular) * objectColor;
      }


      FragColor = vec4(result, 1.0);

}