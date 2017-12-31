#version 330

in vec4 gl_FragCoord;

uniform sampler2D texture0; //albedo
uniform sampler2D texture1; //normals
uniform sampler2D texture2; //position

uniform vec4 Color;
uniform vec3 Center;
uniform vec3 cameraPosition;
uniform vec2 screenSize;
uniform float MaxDistance;

layout(location = 0) out vec4 LightColour;

void main()
{	
	vec2 vertexUV = vec2((gl_FragCoord.x/screenSize.x), (gl_FragCoord.y/screenSize.y));
	vec3 FragPos = texture(texture2, vertexUV).rgb;
	vec3 albedo = texture(texture0, vertexUV).rgb;
	vec3 normal = texture(texture1, vertexUV).rgb * 2 - 1;
	
	float dist = distance(FragPos, Center);
	vec3 lightDir = normalize(Center - FragPos);
	
	float diffuseTerm = clamp(dot(normal, lightDir), 0, 1);
	float distIntensity = clamp(pow((dist/MaxDistance),4), 0, 1);
	
	vec3 finalColor;
	if(FragPos == vec3(0,0,0)) finalColor = vec3(0,0,0);
	
	finalColor = (albedo * diffuseTerm * Color.xyz) * (1-distIntensity);
	
	LightColour += vec4(finalColor, 1);
}