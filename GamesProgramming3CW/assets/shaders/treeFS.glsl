#version 330

flat in int textId;
in vec2 uvsOut;
in vec3 normal;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 Normal;

void main()
{
	switch(textId)
	{
		case 0:
			FragColor = texture(texture0, uvsOut);
			break;
		case 1:
			FragColor = texture(texture1, uvsOut);
			break;
		case 2:
		{
			vec4 texelColor = texture(texture2, uvsOut);
			if(texelColor.a < 0.5)
				discard;
			FragColor = texelColor;
		}
			break;
	}
	Normal = vec4(normal, FragColor.a);
	Normal = Normal * 0.5 + 0.5;
}