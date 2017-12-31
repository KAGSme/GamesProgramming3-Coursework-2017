#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec2 vertexTexCoords;
layout(location = 3) in vec3 vertexNormal;
layout(location = 4) in vec3 vertexTangent;
layout(location = 5) in vec3 vertexBinormal;

out vec2 vertexTexCoordsOut;
out vec3 fragPosOut;
out vec3 worldNormal;

out vec4 shadowCoordsOut;

uniform mat4 MVP;
uniform mat4 Model;

uniform mat4 shadowDepthBiasMVP;

void main()
{
	vec4 worldPos = Model * vec4(vertexPosition, 1.0);

	vertexTexCoordsOut = vertexTexCoords;
	gl_Position = MVP * vec4(vertexPosition, 1.0);
	shadowCoordsOut = shadowDepthBiasMVP * vec4(worldPos.xyz, 1.0);
	fragPosOut = worldPos.xyz;
	worldNormal = normalize(Model*vec4(vertexNormal, 0.0f)).xyz;
}