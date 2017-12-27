#pragma once
#include "BaseComponent.h"
class Enemy :
	public BaseComponent
{
public:
	Enemy();
	~Enemy();

	void OnBegin();
	void Update(float deltaTime);
};

