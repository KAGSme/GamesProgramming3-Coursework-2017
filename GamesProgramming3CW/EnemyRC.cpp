#include "EnemyRC.h"



EnemyRC::EnemyRC(float zLength, float zSpeed)
{
	speed = 50.f;
	life = 20.f;
	_zLength = zLength;
	_zSpeed = zSpeed;
}


EnemyRC::~EnemyRC()
{
	__unhook(&SphereCol::onOverlap, pGameObject->GetCollider(), &EnemyRC::OnOverlap);
}

void EnemyRC::Update(float deltaTime)
{
	vec3 directtion = pGameObject->GetTransform()->GetRight();
	pGameObject->GetTransform()->AddPosition(speed * directtion * deltaTime);

	vec3 pos = pGameObject->GetTransform()->GetPosition();
	float curve = sinf(pGameObject->GetAliveTime() * _zSpeed);
	float newZ = curve * (_zLength / 2);
	pGameObject->GetTransform()->SetPosition(vec3(pos.x, pos.y, newZ));

	life -= deltaTime;
	if (life < 0)
	{
		pGameObject->Destroy();
	}
}
