#pragma once
#include "BaseComponent.h"
#include "Enemy.h"
#include "GameObject.h"
#include "PlayerCar.h"

class EnemySpawner :
	public BaseComponent
{
private:
	float _spawnTimer = 3.f;
	float _spawnTimerMax = 3.f;
	int _spawnAmount = 1;
	float _leftBound;
	float _rightBound;
	vector<vec3> _spawnPositions;
	vector<GameObject*> _enemies;
	PlayerCar* _pc;
public:
	EnemySpawner();
	~EnemySpawner();

	void OnBegin();
	void Update(float deltaTime);

	void GenerateSpawnPositions();
	void SpawnEnemy(int amount);
	void DespawnEnemies();
};

