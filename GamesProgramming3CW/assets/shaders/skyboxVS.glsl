#version 330

layout(location = 0) in vec3 vertexPosition;

out vec3 TexCoords;

uniform mat4 VP;

void main()
{
    gl_Position =  VP * vec4(vertexPosition * 100, 1.0);  
    TexCoords = vertexPosition;
}  