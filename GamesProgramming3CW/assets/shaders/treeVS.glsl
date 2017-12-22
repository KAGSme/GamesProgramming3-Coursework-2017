#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 colors;
layout(location = 2) in vec2 uvs;
layout(location = 3) in vec3 normals;
layout(location = 4) in mat4 model;

uniform mat4 VP;

out vec2 uvsOut;
flat out int textId;
out vec3 normal;

void main()
{
	mat4 MVP = VP * model;
	gl_Position =  MVP * vec4(vertexPosition, 1);
	uvsOut = uvs;
	textId = int(colors.x);
	normal = (model * vec4(normals, 0)).xyz;
}