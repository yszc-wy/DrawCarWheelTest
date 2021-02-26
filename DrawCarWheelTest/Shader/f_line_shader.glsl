#version 330 core
out vec4 FragColor;
uniform vec3 objectColor;
in vec3 Colors;
void main()
{

    FragColor = vec4(Colors, 1.0);
 
}