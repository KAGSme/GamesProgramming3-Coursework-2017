#pragma once
#include "BaseComponent.h"
class coin :
	public BaseComponent
{
public:
	coin();
	~coin();
	float speed;
	float life = 10.f;

	void OnBegin();
	void Update(float deltaTime);
	void OnOverlap(GameObject* other);
};

