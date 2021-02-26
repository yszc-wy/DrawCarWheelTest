#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D strip_texture;

void main()
{
  //FragColor=vec4(1.0,1.0,1.0,1.0);
  
  vec4 texture_color=texture(strip_texture,TexCoord);
  if(texture_color.x<=0.1&&texture_color.y<=0.1&&texture_color.z<=0.1)
  {
    discard;
  }
  FragColor=texture_color;
}