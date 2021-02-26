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

uniform float treadR;//将标准轮数据抬高
uniform float widthMax;
uniform vec2 standardTreadMsg[1000];
uniform int standardTreadMsgSize;
uniform float color_gamut;
uniform bool colorMark;

uniform int color_model;
uniform float max_r;
uniform float min_r;

float GetR(float x,float y)
{
  return sqrt(pow(x,2.0)+pow(y,2.0));
}
vec3 GetColorBetweenTwoColor(float ratio,vec3 start_color,vec3 end_color)//接受一个颜色比例(0-1)和两个颜色，线性计算出中间色
{
  if(ratio<0)
  {
     ratio=0;
  }
  if(ratio>1)
  {
    ratio=1;
  }
  vec3 d_color=ratio*(end_color-start_color);
  return start_color+d_color;
}

vec3 GetColor(float ratio)//接受一个颜色比例范围-1-1
{
  if(ratio<-1)
  {
     ratio=-1;
  }
  if(ratio>1)
  {
    ratio=1;
  }
  
  if(ratio>=-1&&ratio<=-0.5)
  {
    return GetColorBetweenTwoColor((ratio-(-1.0))/0.5,vec3(0.0,0.0,1.0),vec3(0.0,1.0,1.0));
  }
  else if(ratio>-0.5&&ratio<=0.0)
  {
     return GetColorBetweenTwoColor((ratio-(-0.5))/0.5,vec3(0.0,1.0,1.0),vec3(0.0,1.0,0.0));
  }
  else if(ratio>0.0&&ratio<=0.5)
  {
    return GetColorBetweenTwoColor((ratio-(0.0))/0.5,vec3(0.0,1.0,0.0),vec3(1.0,1.0,0.0));
  }
  else if(ratio>0.5&&ratio<=1.0)
  {
    return GetColorBetweenTwoColor((ratio-(0.5))/0.5,vec3(1.0,1.0,0.0),vec3(1.0,0.0,0.0));
  }
}
int FindID(float width)
{ 
  int mid, left = 0, right = standardTreadMsgSize - 1;//闭区间[0, n - 1]
	while (left < right)
	{
		mid = left + ((right + 1 - left) >> 1);//向上取整
		if (standardTreadMsg[mid].x < width) left = mid;
		else right = mid - 1;
	}

	if (standardTreadMsg[left].x <= width&&width<=standardTreadMsg[standardTreadMsgSize - 1].x)
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
    vec3 color;
    
    float radian=atan(FragPosLoc.y,FragPosLoc.x);
    radian = radian < 0 ? (radian + 2 * 3.1415926) : radian;
    
    float width=FragPosLoc.z;
    float r=GetR(FragPosLoc.y,FragPosLoc.x);
    
    if(color_model==0)//高度模式
    {

      if(abs(radian-pickRadian)<0.0025&&pickRadian>=0)
      {
        color=vec3(0.0,0.0,0.0);
      }
      else if(abs(width-pickWidth)<0.5)
      {
        color=vec3(0.0,0.0,0.0);
      }
      else if(colorMark)
      {
        float heightRange=max_r-min_r;
        float colorChange=2.0*(((r-min_r)/heightRange)-0.5);
        color=GetColor(colorChange);
      }
      else
      {
        color = objectColor;
      }
    }
    
    if(color_model==1)//对比模式
    {
      int id=FindID(width);
      float dr=r-(standardTreadMsg[id].y+treadR);  //标准轮数据已归0
      
      if(abs(radian-pickRadian)<0.0025&&pickRadian>=0)
      {
        color=vec3(0.0,0.0,0.0);
      }
      else if(abs(width-pickWidth)<0.5)
      {
        color=vec3(0.0,0.0,0.0);
      }
      else if(colorMark)
      {
          if(id==-1)
          {
            color=vec3(0.0,0.0,0.0);
          }
          else if(width>widthMax)
          {
            color=vec3(0.0,1.0,0.0);
          }
          else
            color=GetColor(dr/color_gamut);
      }
      else
      {
        color = objectColor;
      }
    }
      
    result = (ambient + diffuse + specular) * color;
    FragColor = vec4(result, 1.0);
}