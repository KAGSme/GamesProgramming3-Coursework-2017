#version 330

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 Normal;
layout(location = 2) out vec4 Position;

in vec2 vertexTexCoordsOut;
in mat3 TBN;
in vec3 fragPosOut;
in vec4 shadowCoordsOut;

uniform sampler2D texture0; //albedo
uniform sampler2D texture1; //normal
uniform sampler2D texture2; //specular
uniform sampler2D texture3; //shadowMap

uniform vec3 lightDirection;
uniform float specularPower;
uniform float specularIntensity;

vec3 worldNormal;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz/fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(texture3, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	float bias = max(0.005 * (1.0 - dot(worldNormal, lightDirection)), 0.0005);  
	
	float shadow = 0.0;
	
	vec2 texelSize = 1.0 / textureSize(texture3, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(texture3, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;
    return shadow;
}  

void main()
{
	worldNormal = texture(texture1, vertexTexCoordsOut).rgb;
	worldNormal = normalize(worldNormal * 2.0 - 1.0);   
	worldNormal = normalize(TBN * worldNormal); 
	
	float visibility = ShadowCalculation(shadowCoordsOut);

	FragColor = vec4(texture(texture0, vertexTexCoordsOut).rgb, texture(texture2, vertexTexCoordsOut).r * specularIntensity);
	
	Position.a = (specularPower); //specular
	
	Normal = vec4(worldNormal, visibility);
	
	Position.xyz = fragPosOut;
}
