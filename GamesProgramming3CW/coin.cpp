#include "coin.h"
#include "GameObject.h"
#include "Game.h"
#include "PlayerCar.h"

coin::coin()
{
	speed = 30.f;
	life = 20.f;
}


coin::~coin()
{

}

void coin::OnBegin()
{
	pGameObject->AttachCollider(new SphereCol(2.0f));
	__hook(&SphereCol::onOverlap, pGameObject->GetCollider(), &coin::OnOverlap);
	pGameObject->GetTransform()->GetTransformationMatrix();
}

void coin::Update(float deltaTime)
{
	vec3 direction = vec3(1,0,0);
	pGameObject->GetTransform()->AddPosition(speed * direction * deltaTime);
	pGameObject->GetTransform()->AddRotationEuler(vec3(0,deltaTime*180, 0));
	life -= deltaTime;
	if (life < 0)
	{
		pGameObject->Destroy();
	}
}

void coin::OnOverlap(GameObject * other)
{
	PlayerCar* pc = dynamic_cast<PlayerCar*>(other->GetComponent("PlayerCar"));
	if (pc != NULL)
	{
		Game::GetResourceManager()->GetSound("coinPickUp.wav")->playAudio(AL_NONE);
		pc->AddScore(1000);

		pGameObject->Destroy();
	}
}
