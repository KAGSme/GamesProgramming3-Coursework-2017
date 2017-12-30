#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include "Common.h"
#include "BaseComponent.h"
#include "Renderer.h"
#include "Light.h"
#include "Camera.h"
#include "Transform.h"
#include "Collider.h"
#include "Material.h"
#include <map>

class GameObject
{
private:
	Renderer *renderer = NULL;
	Light *light = NULL;
	map<string, BaseComponent*> components;
	string name = "default";
	Transform* _transform;
	SphereCol* _col;
	Material* _material = new Material();
	bool _pendingDestroy = false;
	bool _isActive = true;

public:
	GameObject();
	~GameObject();

	void SetName(string n) { name = n; }
	string GetName() { return name; }

	Renderer* GetRenderer() { return renderer; }
	Light* GetLight() { return light; }
	Transform* GetTransform() { return _transform; }
	SphereCol* GetCollider() { return _col; }
	Material* GetMaterial() { return _material; }
	void AttachCollider(SphereCol* newCol);

	void Begin();
	void Update(float deltaTime);
	void Render(Camera *camera);
	void Render(Camera *camera, ShaderProgram *overrideProgram);

	void Destroy() { _pendingDestroy = true; };
	bool IsPendingDestroy() { return _pendingDestroy; };

	void SetActive(bool active) { _isActive = active; }
	bool GetActive() { return _isActive; }

	void AttachComponent(BaseComponent *com);
	BaseComponent* GetComponent(string componentType);
	bool _centerModel = true;

};

#endif