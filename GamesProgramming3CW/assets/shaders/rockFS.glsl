#version 330

in vec2 uvsOut;
in vec3 normal;

uniform sampler2D texture0;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 Normal;

void main()
{
	FragColor = texture(texture0, uvsOut);
	//FragColor = vec4(uvsOut, 0, 1);
	Normal = normal * 0.5 + 0.5;
}