#version 330

in vec4 ranges;
in vec2 uvsOut;
in vec3 normal;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 Normal;

void main()
{
	FragColor = texture(texture0, uvsOut) * ranges.x + 
			texture(texture1, uvsOut) * ranges.y + 
			texture(texture2, uvsOut) * ranges.z;
	Normal = normal * 0.5 + 0.5;
}