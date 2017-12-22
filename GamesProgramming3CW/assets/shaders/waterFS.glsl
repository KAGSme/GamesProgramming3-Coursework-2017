#version 330

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 Normal;
layout(location = 2) out vec3 Position;

in vec2 vertexTexCoordsOut;
in vec3 tangentEye;
in vec3 tangentPos;
in vec3 fragPosOut;
in vec4 shadowCoordsOut;

uniform vec3 lightDirection;
uniform sampler2D texture0; //albedo
uniform sampler2D texture1; //normal map
uniform sampler2D texture2; //heightmap
uniform sampler2D texture3;

uniform float heightScale;
uniform float timer;
  
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    float height = texture(texture2, texCoords).r;    
    vec2 p = viewDir.xy / viewDir.z * (height);// * heightScale);
    return texCoords - p;    
} 

vec3 worldNormal;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(texture3, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	float bias = max(0.05 * (1.0 - dot(worldNormal, lightDirection)), 0.005);  
	
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
	//vec2 newVertexTexCoordsOut = vec2(vertexTexCoordsOut.x, vertexTexCoordsOut.y + timer);
	vec3 viewDir = normalize(tangentEye - tangentPos);
    vec2 texCoords = ParallaxMapping(vertexTexCoordsOut, viewDir);
	
	//vec3 worldNormal = texture(texture1, texCoords).rgb;
	worldNormal = texture(texture1, vertexTexCoordsOut).rgb;
	worldNormal = normalize(worldNormal * 2.0 - 1.0);
	
	float visibility = ShadowCalculation(shadowCoordsOut);

	FragColor = vec4(texture(texture0, vertexTexCoordsOut).rgb, 0);
	Normal = vec4(worldNormal, visibility);;
	Position = fragPosOut;
}
