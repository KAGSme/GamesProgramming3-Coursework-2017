#pragma once
#include "BaseComponent.h"
#include "FollowComponent.h"
#include "CameraComponent.h"

class PlayerCar :
	public BaseComponent
{
private:
	CameraComponent* _ThirdPersonCam;
	CameraComponent* _FirstPersonCam;
	bool firstPerson = false;
	float strafeAccel = 0;
	vec3 velocity = vec3(0.0f, 0.0f, 0.0f);
	vec3 accel;
	float maxStrafeVel = 50;
	float friction = 10.0f;
	vec3 newRight;
	void strafe(float state);
	int health = 3;
	float rightBound;
	float leftBound;
	float _timeScore = 0;
	int _otherScore = 0;
	int _finalScore = 0;
	bool debug = false;

public:
	PlayerCar();
	~PlayerCar();

	void OnBegin();
	void Update(float deltaTime);
	void OnRender(Camera *camera);
	void AddHealth(int amount);
	void AddScore(int amount) { _otherScore += amount; }
	void SetScore(int score) { _otherScore = score; _timeScore = 0.f; }
	int GetScore() { return _finalScore; }
	void Reset();

	void switchCam(bool state);
	void FreeCam(bool state);
};

