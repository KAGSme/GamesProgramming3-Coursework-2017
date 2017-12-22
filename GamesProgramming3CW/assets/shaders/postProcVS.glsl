#version 330

layout(location = 0) in vec2 vertexPosition;

out vec2 vertexUV;

void main()
{
	vertexUV = (vertexPosition + 1.0) / 2.0;
	gl_Position = vec4(vertexPosition, 0, 1);
}