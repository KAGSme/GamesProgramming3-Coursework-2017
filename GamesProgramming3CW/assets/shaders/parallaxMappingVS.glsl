#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec2 vertexTexCoords;
layout(location = 3) in vec3 vertexNormal;
layout(location = 4) in vec3 vertexTangent;
layout(location = 5) in vec3 vertexBinormal;

out vec2 vertexTexCoordsOut;
out mat3 TBN;
out vec3 tangentViewPos;
out vec3 fragPos;
out vec3 tangentFragPos;

uniform mat4 MVP;
uniform mat4 Model;
uniform vec3 cameraPosition;

void main()
{
	fragPos = vec3(Model * vec4(vertexPosition, 1.0));   
	vec3 T = normalize(vec3(Model * vec4(vertexTangent, 0.0)));
	vec3 N = normalize(vec3(Model * vec4(vertexNormal, 0.0)));
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
	vec3 B = cross(T, N);

	TBN = mat3(T, B, N);  
	vertexTexCoordsOut = vertexTexCoords;
	tangentViewPos = TBN * cameraPosition;
	tangentFragPos = TBN * fragPos;
	gl_Position = MVP * vec4(vertexPosition, 1.0);
}