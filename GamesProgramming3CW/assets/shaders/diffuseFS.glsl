#version 330

out vec4 FragColor;

in vec3 worldNormal;

uniform vec3 lightDirection;
uniform vec4 ambientMaterialColor;
uniform vec4 diffuseMaterialColor;
uniform vec4 ambientLightColor;
uniform vec4 diffuseLightColor;

void main()
{
	float diffuseTerm = dot(worldNormal, lightDirection);

	FragColor = (ambientMaterialColor*ambientLightColor)+(diffuseMaterialColor*diffuseLightColor*diffuseTerm);
}
