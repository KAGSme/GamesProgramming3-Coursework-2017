#include "WaterComp.h"
#include "GameObject.h"

WaterComp::WaterComp()
{
	timer = 0;
}

WaterComp::~WaterComp() 
{
}

void WaterComp::Update(float deltaTime) 
{
	timer += deltaTime * 0.1; 
}

void WaterComp::OnRender(Camera * camera)  
{ 
	Renderer *r = dynamic_cast<Renderer*>(pGameObject->GetComponent("Renderer"));
	ShaderProgram *s = r->GetProgram();
	s->SetUniform("timer", &timer);
}
