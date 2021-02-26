#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normals;  
uniform sampler2D font_texture;
uniform vec3 objectColor;
void main()
{
    // vec4 color=texture(font_texture,TexCoord);

  // FragColor=vec4(1.0,1.0,1.0,0.0);
  //FragColor.a=0.0;
   // FragColor=vec4(1.0,1.0,1.0,1.0);
  
  vec4 texture_color=texture(font_texture,TexCoord);
  if(texture_color.x==0)
  {
    discard;
  }
  //mix(texture(texture1, TexCoord1), texture(texture2, TexCoord2), alp);
  FragColor=texture_color*vec4(objectColor,1.0f);
  
}