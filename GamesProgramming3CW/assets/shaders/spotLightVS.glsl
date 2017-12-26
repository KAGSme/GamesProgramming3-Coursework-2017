#version 330

layout(location = 0) in vec3 Position;

uniform mat4 Model;
uniform mat4 MVP;

out vec3 WorldPos;

void main()
{
	gl_Position = MVP * vec4(Position, 1);
	WorldPos = gl_Position.xyz;
}