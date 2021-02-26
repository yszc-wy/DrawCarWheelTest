#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoords;


//out vec2 TexCoords;
out vec3 Normals;
out vec3 FragPos;
out vec3 FragPosLoc;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    //TexCoords = aTexCoords;   
    Normals=mat3(transpose(inverse(model))) *aNormals;
    FragPos=vec3(model*vec4(aPos,1.0));
    FragPosLoc=aPos;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}