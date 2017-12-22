#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 3) in vec3 vertexNormal;

out vec3 worldNormal;
out vec3 cameraDirectionOut;
out vec3 FragPos;

out vec4 shadowCoordsOut;

uniform mat4 MVP;
uniform mat4 Model;
uniform vec3 cameraPosition;

uniform mat4 shadowDepthBiasMVP;

void main()
{
	worldNormal = normalize(Model*vec4(vertexNormal, 0.0f)).xyz;
	vec3 worldPos = (Model*vec4(vertexPosition, 1.0)).xyz;
	cameraDirectionOut = normalize(cameraPosition - worldPos);

	shadowCoordsOut = shadowDepthBiasMVP * vec4(worldPos, 1.0);
	gl_Position = MVP * vec4(vertexPosition, 1.0);
	FragPos = worldPos;
}