#version 330

in vec2 vertexUV;

uniform sampler2D texture0;

out vec4 FragColor;

void main()
{
	vec4 color = texture(texture0, vertexUV);
	FragColor = color.bgra;
}