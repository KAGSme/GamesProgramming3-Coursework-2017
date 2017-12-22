#version 330

out vec4 FragColor;

in vec4 vertexColorOut;

uniform vec4 ambientMaterialColor;
uniform vec4 ambientLightColor;

void main()
{
	FragColor = ambientMaterialColor * ambientLightColor;
}