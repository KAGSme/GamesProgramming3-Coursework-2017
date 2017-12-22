#ifndef _BASECOMPONENT_H
#define _BASECOMPONENT_H

//forward declaring to awoid circular includes
class GameObject;
class Camera;

class BaseComponent
{
protected:
	GameObject *pGameObject;

public:
	BaseComponent();
	~BaseComponent();

	virtual void SetParentGO(GameObject *pGO);
	GameObject* GetParentGO() { return pGameObject; };

	virtual void OnBegin() {}
	virtual void Update(float deltaTime) {}
	virtual void OnRender(Camera *camera) {}
};
#endif