#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec2 vertexTexCoords;
layout(location = 3) in vec3 vertexNormal;
layout(location = 4) in vec3 vertexTangent;
layout(location = 5) in vec3 vertexBitangent;

out vec2 vertexTexCoordsOut;
out vec3 tangentEye;
out vec3 tangentPos;
out vec3 fragPosOut;

out vec4 shadowCoordsOut;

uniform mat4 MVP;
uniform mat4 Model;
uniform vec3 cameraPosition;

uniform mat4 shadowDepthBiasMVP;

void main()
{
	//getting the basics done
	gl_Position = MVP * vec4(vertexPosition, 1.0);
	vertexTexCoordsOut = vertexTexCoords;

	//getting our world-to-tangent matrix
	vec3 T = mat3(Model) * vertexTangent;
	vec3 B = mat3(Model) * vertexBitangent;
	vec3 N = mat3(Model) * vertexNormal;
	mat3 TBN = transpose(mat3(T, B, N));
	
	vec3 worldPos = vec3(Model * vec4(vertexPosition, 1.0));

	tangentPos = TBN * vec3(Model * vec4(vertexPosition, 1));
	tangentEye = TBN * cameraPosition;
	shadowCoordsOut = shadowDepthBiasMVP * vec4(worldPos, 1.0);
	fragPosOut = worldPos;
}