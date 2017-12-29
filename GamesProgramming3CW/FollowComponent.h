#pragma once
#include "BaseComponent.h"
#include "Common.h"
class FollowComponent :
	public BaseComponent
{
private:
	vec3 _offsetPos;
	GameObject *_followGo;
	string _followName;

public:
	FollowComponent();
	FollowComponent(vec3 offsetPosition, string follow);
	~FollowComponent();

	void Update(float deltaTime);
	void OnBegin();
};

