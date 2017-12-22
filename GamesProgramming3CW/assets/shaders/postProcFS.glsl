#version 330

in vec2 vertexUV;

uniform sampler2D texture0;

out vec4 FragColor;

void main()
{
	FragColor = texture(texture0, vertexUV);
}