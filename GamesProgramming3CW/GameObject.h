#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include "Common.h"
#include "BaseComponent.h"
#include "Renderer.h"
#include "Light.h"
#include "Camera.h"
#include "Transform.h"
#include <map>

class GameObject
{
private:
	Renderer *renderer = NULL;
	Light *light = NULL;
	map<string, BaseComponent*> components;
	string name = "default";
	Transform* _transform;

public:
	GameObject();
	~GameObject();

	void SetName(string n) { name = n; }
	string GetName() { return name; }

	Renderer* GetRenderer() { return renderer; }
	Light* GetLight() { return light; }
	Transform* GetTransform() { return _transform; }

	void Begin();
	void Update(float deltaTime);
	void Render(Camera *camera);
	void Render(Camera *camera, ShaderProgram *overrideProgram);

	void AttachComponent(BaseComponent *com);
	BaseComponent* GetComponent(string componentType);
};

#endif