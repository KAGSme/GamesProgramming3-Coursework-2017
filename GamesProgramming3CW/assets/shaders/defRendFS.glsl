#version 330

in vec2 vertexUV;

uniform sampler2D texture0; //albedo
uniform sampler2D texture1; //normals
uniform sampler2D texture2; //position
uniform sampler2D texture3; //depth

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientLightColor;
uniform vec3 specularLightColor;
uniform vec3 cameraPosition;

out vec4 FragColor;

//The goal of this shader is to join up all the textures in to the final image(albedo/normal/depth/specular)
void main()
{
	vec3 FragPos = texture(texture2, vertexUV).rgb;
	vec3 albedo = texture(texture0, vertexUV).rgb;
	vec3 normal = texture(texture1, vertexUV).rgb * 2 - 1;
	float shadowVisibility = texture(texture1, vertexUV).a;
	float spec = texture(texture0, vertexUV).a;
	float shine = texture(texture2, vertexUV).a;
	
	float diffuseTerm = clamp(dot(normal, lightDir), 0, 1);
	
	vec3 viewDir = normalize(cameraPosition - FragPos);
	vec3 halfWayVec = normalize(viewDir + lightDir);
	float specularTerm = pow(clamp(dot(normal , halfWayVec), 0.f, 1.f), shine);
	
	vec3 finalColor;
	if(FragPos == vec3(0,0,0)) finalColor = albedo;
	
	else finalColor = (ambientLightColor * albedo) + 
	(albedo * diffuseTerm * lightColor * (1 - shadowVisibility)) + 
	(spec * specularTerm * specularLightColor * (1 - shadowVisibility));
	
	FragColor = vec4(finalColor, 1);
}