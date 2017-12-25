#version 330

in vec4 gl_FragCoord;

uniform sampler2D texture0; //albedo
uniform sampler2D texture1; //normals
uniform sampler2D texture2; //position

uniform vec4 Color;
uniform vec3 Center;
uniform float Intensity;
uniform float MaxDistance;
uniform vec3 cameraPosition;

layout(location = 0) out vec4 LightColour;

void main()
{	
	vec2 vertexUV = vec2((gl_FragCoord.x/1280), (gl_FragCoord.y/768));
	vec3 FragPos = texture(texture2, vertexUV).rgb;
	vec3 albedo = texture(texture0, vertexUV).rgb;
	vec3 normal = texture(texture1, vertexUV).rgb * 2 - 1;
	float spec = texture(texture0, vertexUV).a;
	float shine = texture(texture2, vertexUV).a;
	
	float dist = distance(FragPos, Center);
	vec3 lightDir = normalize(Center - FragPos);
	
	float diffuseTerm = clamp(dot(normal, lightDir), 0, 1);
	
	vec3 viewDir = normalize(cameraPosition - FragPos);
	vec3 halfWayVec = normalize(viewDir + lightDir);
	float specularTerm = pow(clamp(dot(normal , halfWayVec), 0.f, 1.f), shine);
	
	vec3 finalColor;
	if(FragPos == vec3(0,0,0)) finalColor = vec3(0,0,0);
	
	float distIntensity = clamp((dist/MaxDistance), 0, 1);
	
	finalColor = (albedo * diffuseTerm * Color.xyz * (1-distIntensity))
	+ (spec * specularTerm * Color.xyz * (1-distIntensity));
	
	LightColour += vec4(finalColor, 1);
}