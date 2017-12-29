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
	if (!_followGo) return;
	vec3 pos = _followGo->GetTransform()->GetPosition() + _offsetPos;
	pGameObject->GetTransform()->SetPosition(pos);
}

void FollowComponent::OnBegin()
{
	_followGo = Game::GetCurrentScene()->GetGameObject(_followName);
}
