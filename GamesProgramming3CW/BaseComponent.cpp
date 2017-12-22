#include "BaseComponent.h"

BaseComponent::BaseComponent()
{
	pGameObject = 0;
}

BaseComponent::~BaseComponent()
{
}

void BaseComponent::SetParentGO(GameObject *pGO)
{
	pGameObject = pGO;
}

//this caused me link errors when calling function
/*inline GameObject* BaseComponent::GetParentGO()
{
	return pGameObject;
}*/
