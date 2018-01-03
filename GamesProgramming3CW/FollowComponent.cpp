#include "FollowComponent.h"
#include "GameObject.h"
#include "Game.h"


FollowComponent::FollowComponent()
{
}

FollowComponent::FollowComponent(vec3 offsetPosition, string follow)
{
	_offsetPos = offsetPosition;
	_followName = follow;
}


FollowComponent::~FollowComponent()
{
}

void FollowComponent::Update(float deltaTime)
{
	//simply keep gameobject in same relative world position to the other gameobject
	if (!_followGo) return;
	vec3 pos = _followGo->GetTransform()->GetPosition() + _offsetPos;
	pGameObject->GetTransform()->SetPosition(pos);
}

void FollowComponent::OnBegin()
{
	_followGo = Game::GetCurrentScene()->GetGameObject(_followName);
}
