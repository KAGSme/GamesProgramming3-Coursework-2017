#pragma once
#include "Enemy.h"
class EnemyRC :
	public Enemy
{
private:
	float _zLength;
	float _zSpeed = 1;
public:
	EnemyRC(float zLength, float zSpeed);
	~EnemyRC();
	void Update(float deltaTime);
};

