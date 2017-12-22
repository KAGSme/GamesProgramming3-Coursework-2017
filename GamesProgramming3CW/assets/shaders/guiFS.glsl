#version 330

in vec2 uvsOut;

uniform sampler2D texture0;

out vec4 FragColor;

void main()
{
	//FragColor = vec4(1, 1, 1, 1);
	FragColor = texture(texture0, uvsOut);
}