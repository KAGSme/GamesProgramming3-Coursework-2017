#pragma once
#include "BaseComponent.h"
#include "PlayerCar.h"
class Enemy :
	public BaseComponent
{
public:
	Enemy();
	~Enemy();
	float speed;
	float life = 10.f;

	void OnBegin();
	void Update(float deltaTime);
	void OnOverlap(GameObject* other);
};

