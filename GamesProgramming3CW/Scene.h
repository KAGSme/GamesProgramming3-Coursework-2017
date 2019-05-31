#ifndef _SCENE_H
#define _SCENE_H

#include "Common.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "tinyxml2.h"

#include "CameraBehaviour.h"

static enum componentID {
	COMPONENT_NOTFOUND,
	CAMERA_BEHAVIOUR,
	SKYBOX,
	TIMEDAY,
	MOVEGOBEHAVIOUR,
	LIGHT,
	CAMERA_COMPONENT,
	PLAYER_CAR,
	ENEMY,
	FOLLOW_COMP,
	ENEMY_SPAWNER
};

using namespace tinyxml2;

class Scene
{
private:
	string name;
	Camera *camera;
	ResourceManager* resourceManager;
	map<string, componentID> componentIDValues;
	vector<GameObject*> gameObjects;
	vector<GameObject*> visibleGOs;
	vector<GameObject*> transparentGOs;
	vector<GameObject*> lights;
	vector<GameObject*> _newGOs;
	GameObject* mainDirLight;
	GameObject* _skybox;

public:
	Scene(ResourceManager* rM);
	~Scene();

	Camera* GetSceneCamera() { return camera; }

	//adds a new gameobject to the vector
	GameObject* AddGameObject(const string& name, const vec3& position, const vec3& rotation, const vec3& scale, Renderer *r);
	GameObject* AddGameObject(const string& name, const vec3& position, const vec3& rotation, const vec3& scale);
	void AttachComponent(string& compID, GameObject* go, XMLElement* attributesElement);
	//adds a creted gameobject from outside the scene to the scene
	void AddGameObject(GameObject* go);
	//Deletes all the things!
	void AddNewGameObjects();
	void ReleaseResources();
	void Begin();
	void Update(float deltaTime);
	void RemoveDeadGameObjects();
	void VisibilityCheck();
	void Sort(bool(*comparer)(GameObject *a, GameObject *b));
	void Render(Camera* camera);
	void RenderDirShadowMap();

	int GetLightCount() { return lights.size(); }
	Renderer* GetLight(int i) { return lights[i]->GetRenderer(); }

	GameObject* GetMainDirLight() { return mainDirLight; }
	void SetMainDirLight(GameObject* dirL);

	int GetGOCount() { return gameObjects.size(); }
	int GetVisibleGOCount() { return visibleGOs.size(); }

	GameObject* GetGameObject(string name);
};
#endif 
