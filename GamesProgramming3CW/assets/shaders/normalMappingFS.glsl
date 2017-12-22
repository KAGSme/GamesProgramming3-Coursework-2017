#version 330

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 Normal;

in vec2 vertexTexCoordsOut;
in mat3 TBN;

uniform vec3 lightDirection;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	vec3 worldNormal = texture(texture1, vertexTexCoordsOut).rgb;
	worldNormal = normalize(worldNormal * 2.0 - 1.0);   
	worldNormal = normalize(TBN * worldNormal); 

	FragColor = texture(texture0, vertexTexCoordsOut);
	//FragColor = vec4(worldNormal, 0.0);
	Normal = worldNormal;
}
