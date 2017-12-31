#ifndef _GAME_H
#define _GAME_H

#include "Common.h"
#include "Camera.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "GameObject.h"
#include "Model.h"
#include "Font.h"
#include "SceneManager.h"

enum class GameState { PLAY, PAUSE, EXIT };

class Game
{
private:
	SceneManager* sceneManager;

	int fpsCounter = 0;
	int fpsDisplay = 0;
	float fpsTimer = 0;
	static bool debugMode;
	bool negMode = false;
	SDL_Window *_window;
	GameState _currentGameState;
	SDL_GLContext _glContext;
	bool _paused = false;
	SDL_GameController* _gGameController;
	
	int inputTime = 0;
	int inputTicks = 0;

	int renderTime = 0;
	
	int updateTime = 0;
	int updateTicks;

	static float _globaDeltaTime;
	static ResourceManager* resourceManager;
	static Scene* currentScene;

public:
	Game();
	~Game();

	int Run();
	void Loop();

	void LoadResources();
	void ReleaseResources();

	void Update(float deltaTime);
	void Render(float deltaTime);

	static uint verticesRendered;
	static uint drawCalls;

	static Scene* GetCurrentScene() { return currentScene; }
	static ResourceManager* GetResourceManager() { return resourceManager; }
	static float GetGlobalDeltaTime() { return _globaDeltaTime; }
	static bool GetDebugMode() { return debugMode; }
};

#endif