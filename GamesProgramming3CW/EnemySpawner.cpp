#include "EnemySpawner.h"



EnemySpawner::EnemySpawner()
{

}


EnemySpawner::~EnemySpawner()
{
}

void EnemySpawner::OnBegin()
{
	_leftBound = -40.f;
	_rightBound = 40.f;
	_spawnAmount = 1;
	_spawned = 0;

	GenerateSpawnPositions();

	_pc = dynamic_cast<PlayerCar*>(Game::GetCurrentScene()->GetGameObject("PlayerCar")->GetComponent("PlayerCar"));

	InputAction* ia = Input::GetInputActionState("Restart");
	if (ia)
		__hook(&InputAction::InputActionChange, ia, &EnemySpawner::ResetGame);

	Game::GetResourceManager()->GetSound("Slammin.wav")->playAudio(AL_NONE);
}

void EnemySpawner::Update(float deltaTime)
{
	Sound* sound = Game::GetResourceManager()->GetSound("Slammin.wav");
	if (sound->GetState() == AL_STOPPED)
		sound->playAudio(AL_NONE);

	if (!pGameObject->GetActive())
	{
		Game::GetResourceManager()->GetFont("Regensburg.ttf")->Render("Game Over!!! Restart By pressing R", { 100, (int)SCREEN_H/2, 650, 50 });
		string score = "Score: " + to_string(_pc->GetScore());
		int scoreLength = score.size();
		Game::GetResourceManager()->GetFont("Regensburg.ttf")->Render(score, { 100, (int)SCREEN_H / 2 - 50, static_cast<__int64>(scoreLength) * 22, 50 });
		return;
	}

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
	//increase enemies after every 10 that get spawned
	_spawnAmount = clamp(_spawned / 10, 1, (int)_spawnPositions.size() - 2); 
}

void EnemySpawner::GenerateSpawnPositions()
{
	int maxSpawnPositions = ((_rightBound-4) - (_leftBound+4)) / 8;//8 is the diameter of the enemy cars (see enemy.cpp)
	vec3 posXY = pGameObject->GetTransform()->GetPosition();
	for (int i = 0; i < maxSpawnPositions; i++)
	{
		_spawnPositions.push_back(vec3(posXY.x, posXY.y, (i * 8) + _leftBound+4));
	}
}

void EnemySpawner::SpawnEnemy(int amount)
{
	if (rand() % 99 < 80)
	{
		//random number series
		vector<int> rShuffle;
		for (int i = 0; i < _spawnPositions.size(); i++)
		{
			rShuffle.push_back(i);
		}
		for (int i = rShuffle.size(); i > 0; --i)
		{
			int temp = rShuffle.at(i - 1);
			int j = rand() % i;
			rShuffle.at(i - 1) = rShuffle.at(j);
			rShuffle.at(j) = temp;
		}
		//spawn a coin
		Renderer* renderer = new Renderer();
		ShaderProgram *shader = Game::GetResourceManager()->GetShader("diffuseNormalSpecMap");
		renderer->SetShaderProgram(shader);
		Model *m = Game::GetResourceManager()->GetModel("coin/coin.fbx");
		renderer->SetModel(m, GL_TRIANGLES);
		renderer->AddTexture(Game::GetResourceManager()->GetTexture("coin-texture.jpg"));
		renderer->AddTexture(Game::GetResourceManager()->GetTexture("coin-Normal.png"));
		renderer->AddTexture(Game::GetResourceManager()->GetTexture("coin-Spec.png"));

		GameObject *coinGo = Game::GetCurrentScene()->AddGameObject("coin" + to_string(_spawned),
			_spawnPositions.at(rShuffle.at(0)), vec3(0, 0, 0), vec3(2, 2, 0.1), renderer);
		coin* Coin = new coin();
		coinGo->AttachComponent(Coin);
		_enemies.push_back(coinGo);

		for (int i = 1; i < amount+1; i++)
		{
			Renderer* renderer = new Renderer();
			ShaderProgram *shader = Game::GetResourceManager()->GetShader("carSUV");
			renderer->SetShaderProgram(shader);
			Model *m = Game::GetResourceManager()->GetModel("carSUV.fbx");
			renderer->SetModel(m, GL_TRIANGLES);
			renderer->AddTexture(Game::GetResourceManager()->GetTexture("Car/carSUV_COLOR.jpg"));
			renderer->AddTexture(Game::GetResourceManager()->GetTexture("Car/carSUV_NORMAL.jpg"));

			GameObject *enemyGo = Game::GetCurrentScene()->AddGameObject("enemy" + to_string(_spawned),
				_spawnPositions.at(rShuffle.at(i)), vec3(-90, -90, 0), vec3(3, 5, 3), renderer);
			Enemy* enemy = new Enemy();
			enemyGo->AttachComponent(enemy);
			_enemies.push_back(enemyGo);
			_spawned++;
		}
	}
	else
	{
		Renderer* renderer = new Renderer();
		ShaderProgram *shader = Game::GetResourceManager()->GetShader("diffuseNormalSpecMap");
		renderer->SetShaderProgram(shader);
		Model *m = Game::GetResourceManager()->GetModel("RegularC.FBX");
		renderer->SetModel(m, GL_TRIANGLES);
		renderer->AddTexture(Game::GetResourceManager()->GetTexture("Car/RegularC_diffuse_green.png"));
		renderer->AddTexture(Game::GetResourceManager()->GetTexture("Car/RegularC_normal.png"));
		renderer->AddTexture(Game::GetResourceManager()->GetTexture("Car/RegularC_specular.png"));

		GameObject *enemyGo = Game::GetCurrentScene()->AddGameObject("enemyRC" + to_string(_spawned),
			_spawnPositions.at(_spawnPositions.size()-1), vec3(-90, 0, 0), vec3(3, 5, 3), renderer);
		Enemy* enemy = new EnemyRC(_rightBound - _leftBound, _spawnAmount);
		enemyGo->AttachComponent(enemy);
		_enemies.push_back(enemyGo);
		_spawned++;
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

void EnemySpawner::ResetGame(bool state)
{
	if (!pGameObject->GetActive())
	{
		pGameObject->SetActive(true);
		_pc->GetParentGO()->SetActive(true);
		vec3 pcPos = _pc->GetParentGO()->GetTransform()->GetPosition();
		_pc->GetParentGO()->GetTransform()->SetPosition(vec3(pcPos.x, pcPos.y, pcPos.z));
		_pc->AddHealth(3);
		_pc->SetScore(0);
		_spawnAmount = 1;
		_spawned = 0;
	}
}
