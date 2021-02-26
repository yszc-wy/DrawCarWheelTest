#version 330 core
out vec4 FragColor;

in vec3 Normals;  
in vec3 FragPos;  
in vec3 FragPosLoc;
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform float pickLength;
uniform float pickWidth;
uniform float unitLength;
uniform float unitWidth;
uniform float widthMax;

//uniform float originHeight;
uniform float max_y;
uniform float min_y;
uniform vec2 standardTreadMsg[1000];  //多少才够？
uniform int standardTreadMsgSize;
uniform float color_gamut;
uniform int color_model;

/*
vec3 GetColor(float colorChange)
{
  if(colorChange<-1)
  {
    colorChange=-1;
  }
  if(colorChange>1)
  {
    colorChange=1;
  }
  
  vec3 color=vec3(0.0,1.0,0.0);
  if(colorChange<0)
  {
    color=vec3(0.0,1.0+colorChange,0.0-colorChange);
  }
  else if(colorChange>0)
  {
    color=vec3(0.0+colorChange,1.0-colorChange,0.0);
  }
  return color;
}
*/
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
    
    vec3 lightDir;
    float diff;
    vec3 diffuse;
    if(gl_FrontFacing)
    {
      lightDir = normalize(lightPos - FragPos);
      diff = max(dot(norm, lightDir), 0.0);
      diffuse = diff * lightColor;
    } else{
      lightDir = normalize(lightPos - FragPos);
      diff = max(dot(-norm, lightDir), 0.0);
      diffuse = diff * lightColor;
    }
    
    // specular
    float specularStrength = 0.0;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result;
    vec3 color;
    
    
    if(color_model==0)//高度模式
    {
      //float originY=FragPosLoc.y+originHeight;
      if(abs(FragPosLoc.z-pickWidth)<=0.5)//标线显示
      {
          color=vec3(0.0,0.0,0.0);
      }
      else if(abs(FragPosLoc.x-pickLength)<=0.5)//标线显示
      {
          color=vec3(0.0,0.0,0.0);
      }
      else
      {
        float heightRange=max_y-min_y;
        float colorChange=2.0*(((FragPosLoc.y-min_y)/heightRange)-0.5);
        color=GetColor(colorChange);
      }
    }
    //按标准轮对比情况赋颜色
    if(color_model==1)//对比模式
    {
       //float originY=FragPosLoc.y+originHeight;
      //float heightRange=max_y-min_y;
      //float colorChange=2.0*(originY-min_y)/heightRange;
      
      float width=FragPosLoc.z;
      int id=FindID(width);
      float dy=FragPosLoc.y-standardTreadMsg[id].y;
      
      if(abs(FragPosLoc.z-pickWidth)<=0.5)//标线显示
      {
          color=vec3(0.0,0.0,0.0);
      }
      else if(abs(FragPosLoc.x-pickLength)<=0.5)//标线显示
      {
          color=vec3(0.0,0.0,0.0);
      }
      else
      {
          //color=objectColor;
          if(id==-1)
          {
            color=vec3(0.0,0.0,0.0);
          }
          else if(width>widthMax)
          {
            color=vec3(0.0,1.0,0.0);
          }
          else
            color=GetColor(dy/color_gamut);
          
      }
    }
    
    
    //vec3 testColor=GetColor(colorChange);
    //if(abs(originHeight-0.0)<0.1)
    //{
    // color=vec3(1.0,0.0,0.0);
    //}
    
    //color=vec3(0.0,1.0,0.0);
    
    result = (ambient + diffuse + specular) * color;

    FragColor = vec4(result, 1.0);

}