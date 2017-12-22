#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 colors;
layout(location = 2) in vec2 uvs;
layout(location = 3) in vec3 normals;

uniform mat4 Model;
uniform mat4 VP;

out vec2 uvsOut;
out vec4 ranges;
out vec3 normal;

void main()
{
	gl_Position = VP * Model * vec4(vertexPosition, 1);
	uvsOut = uvs;
	ranges = colors;
	normal = (Model * vec4(normals, 0)).xyz;
}