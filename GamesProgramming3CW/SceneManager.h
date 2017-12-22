#ifndef _SCENEMANAGER_H
#define _SCENEMANAGER_H

#include "Common.h"
#include "Scene.h"
#include "tinyxml2.h"
#include "Font.h"
#include "CommonXml.h"
#include <map>

using namespace tinyxml2;

class SceneManager 
{
private:
	XMLError eResult;
	string sceneMgrConfigPath;
	ResourceManager* resourceManager;
	
public:
	map<int, string> scenes;
	SceneManager(ResourceManager* rM);
	~SceneManager();
	//loads the scene directories for all the scenes in the deliverable
	XMLError LoadSceneDirectories();
	//loads the scene, takes in the scene order int as a parameter
	//scenes loaded can only contain basic gameobjects with no components just now
	XMLError LoadScene(int lvlOrder, Scene* currentScene);
	//loads the scene, takes in the scene name(without the ".scn") as a parameter
	XMLError LoadScene(string lvlName, Scene* currentScene);
};
#endif 