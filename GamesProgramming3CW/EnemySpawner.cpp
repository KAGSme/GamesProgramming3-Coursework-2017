#include "EnemySpawner.h"



EnemySpawner::EnemySpawner()
{

}


EnemySpawner::~EnemySpawner()
{
}

void EnemySpawner::OnBegin()
{
	_leftBound = -30.f;
	_rightBound = 30.f;
	_spawnAmount = 1;

	GenerateSpawnPositions();

	_pc = dynamic_cast<PlayerCar*>(Game::GetCurrentScene()->GetGameObject("PlayerCar")->GetComponent("PlayerCar"));
}

void EnemySpawner::Update(float deltaTime)
{
	if (!pGameObject->GetActive()) return;

	_spawnTimer -= deltaTime;
	if (_spawnTimer < 0)
	{
		_spawnTimer = _spawnTimerMax;

		SpawnEnemy(_spawnAmount);
	}

	if (_pc)
	{
		if (!_pc->GetParentGO()->GetActive())
		{
			DespawnEnemies();
			pGameObject->SetActive(false);
		}
	}
}

void EnemySpawner::GenerateSpawnPositions()
{
	int maxSpawnPositions = ((_rightBound-4) - (_leftBound+4)) / 8;//8 is the diameter of the enemy cars (see enemy.cpp)
	vec3 posXY = pGameObject->GetTransform()->GetPosition();
	for (int i = 0; i < maxSpawnPositions; i++)
	{
		_spawnPositions.push_back(vec3(posXY.x, posXY.y, (i * 8) + _leftBound));
	}
}

void EnemySpawner::SpawnEnemy(int amount)
{
	for (int i = 0; i < amount; i++)
	{
		Renderer* renderer = new Renderer();
		ShaderProgram *shader = Game::GetResourceManager()->GetShader("diffuseNormalSpecMap");
		renderer->SetShaderProgram(shader);
		Model *m = Game::GetResourceManager()->GetModel("carSUV.fbx");
		renderer->SetModel(m, GL_TRIANGLES);
		renderer->AddTexture(Game::GetResourceManager()->GetTexture("Car/carSUV_COLOR.jpg"));
		renderer->AddTexture(Game::GetResourceManager()->GetTexture("Car/carSUV_NORMAL.jpg"));

		GameObject *enemyGo = Game::GetCurrentScene()->AddGameObject("enemy" + to_string(_spawnPositions.size()),
			_spawnPositions.at(rand() % _spawnPositions.size()), vec3(-90, -90, 0), vec3(3, 5, 3), renderer);
		Enemy* enemy = new Enemy();
		enemyGo->AttachComponent(enemy);
		_enemies.push_back(enemyGo);
	}
}

void EnemySpawner::DespawnEnemies()
{
	for (auto iter = _enemies.begin(); iter != _enemies.end(); iter++)
	{
		if ((*iter))
		{
			(*iter)->Destroy();
		}
	}
	_enemies.clear();
}
