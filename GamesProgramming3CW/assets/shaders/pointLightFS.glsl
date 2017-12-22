#version 330

in vec3 WorldPos;

uniform vec4 Color;
uniform vec3 Center;
uniform float Intensity;

out vec4 FragColor;

void main()
{
	float dist = distance(WorldPos, Center);
	float coeff = Intensity / dist;
	FragColor = Color * coeff;
}