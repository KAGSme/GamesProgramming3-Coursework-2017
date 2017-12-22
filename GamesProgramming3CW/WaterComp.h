#pragma once
#include "BaseComponent.h"
#include "Common.h"
class WaterComp :
	public BaseComponent
{
private:
	float timer;
public:
	WaterComp();
	~WaterComp();

	void Update(float deltaTime);
	void OnRender(Camera *camera);
};

