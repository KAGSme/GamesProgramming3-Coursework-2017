#version 330

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 Normal;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform samplerCube skyboxNight;
uniform float blendFactor;

void main()
{    
	FragColor = mix(texture(skybox, TexCoords), texture(skyboxNight, TexCoords), blendFactor);
	Normal = vec3(0.5, 0.5, 0.5);
}