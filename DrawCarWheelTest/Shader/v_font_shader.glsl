#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoord;
out vec3 Normals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    TexCoord=aTexCoords;
    Normals=mat3(transpose(inverse(model))) *aNormals;
    
    gl_Position = projection*view*model*vec4(aPos, 1.0f);
}