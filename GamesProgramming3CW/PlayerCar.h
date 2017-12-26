#pragma once
#include "BaseComponent.h"
#include "CameraComponent.h"
class PlayerCar :
	public BaseComponent
{
private:
	CameraComponent* _cameraComponent;
	bool isActive = true;
	float strafeAccel = 0;
	vec3 velocity = vec3(0.0f, 0.0f, 0.0f);
	vec3 accel;
	float maxStrafeVel = 50;
	float friction = 10.0f;
	vec3 newRight;
	void strafe(float state);
public:
	PlayerCar();
	~PlayerCar();

	void OnBegin();
	void Update(float deltaTime);
};

