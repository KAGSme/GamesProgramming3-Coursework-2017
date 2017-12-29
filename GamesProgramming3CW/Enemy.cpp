#include "Enemy.h"
#include "Camera.h"
#include "GameObject.h"
#include "Game.h"


Enemy::Enemy()
{
	speed = 8.0f;
}


Enemy::~Enemy()
{
	__unhook(&SphereCol::onOverlap, pGameObject->GetCollider(), &Enemy::OnOverlap);
}

void Enemy::OnBegin()
{
	pGameObject->AttachCollider(new SphereCol(4.0f));
	__hook(&SphereCol::onOverlap, pGameObject->GetCollider(), &Enemy::OnOverlap);
	pGameObject->GetTransform()->GetTransformationMatrix();
}

void Enemy::Update(float deltaTime)
{
	vec3 directtion = pGameObject->GetTransform()->GetForward();
	pGameObject->GetTransform()->AddPosition(speed * directtion * deltaTime);
	//cout << pGameObject->GetTransform()->GetRotationEuler().x << " " << pGameObject->GetTransform()->GetRotationEuler().y << " " << pGameObject->GetTransform()->GetRotationEuler().z << endl;
}

void Enemy::OnOverlap(GameObject * other)
{
	PlayerCar* pc = dynamic_cast<PlayerCar*>(other->GetComponent("PlayerCar"));
	if (pc != NULL)
	{
		pGameObject->Destroy();
		pc->AddHealth(-1);
	}
}
