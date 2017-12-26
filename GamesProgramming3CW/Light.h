#ifndef _LIGHT_H
#define _LIGHT_H

#include "Common.h"
#include "BaseComponent.h"
#include "Camera.h"
#include "Texture.h"

enum class E_LightState {POINT, DIRECTIONAL };

class Light : public BaseComponent
{
private:
	E_LightState _state;
	vec4 color;
	float intensity = 10.0f;
	vec3 direction = {1,1,1};

	GLuint SHADOW_WIDTH = 8192, SHADOW_HEIGHT = 8192;
	GLuint depthMapFBO;
	GLuint depthMap;
	Texture* depthTexture;

	mat4 depthVP;

public:
	Light(vec4 color);
	Light(vec4 color, E_LightState state);
	~Light();

	vec4 GetColor() { return color; }
	float GetIntensity() { return intensity; }
	E_LightState GetLightType(){ return _state; }
	vec3 GetLightDirection(){ return direction; }

	void SetColor(vec4 value) { color = value; }
	void SetDirection(vec3 value) { direction = value; }
	
	GLuint GetShadowMap() { return depthMap; }
	mat4 GetShadowMapDepthVP() { return depthVP; }

	Camera* ConfigureDirLightCamera();
	void DirShadowMapRenderStart();
	void DirShadowMapRenderEnd();

	void OnBegin();
	void Update(float deltaTime);
};

#endif