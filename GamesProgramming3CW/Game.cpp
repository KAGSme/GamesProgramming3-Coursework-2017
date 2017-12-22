#include "Game.h"
#include "Input.h"
#include "Graphics.h"
#include "PostProcessing.h"
#include "CameraBehaviour.h"
#include "TerrainComp.h"
#include "DefRenderer.h"

uint Game::verticesRendered;
uint Game::drawCalls;
Scene* Game::currentScene;
ResourceManager* Game::resourceManager;

#define SKYBOX_DIST 5

vector<Vertex> skyBoxverts = {
	//Front
	{ vec3(-SKYBOX_DIST, SKYBOX_DIST, SKYBOX_DIST),
	vec4(0), vec2(0.0f, 0.0f) },// Top Left

	{ vec3(-SKYBOX_DIST, -SKYBOX_DIST, SKYBOX_DIST),
	vec4(0), vec2(0.0f, 1.0f) },// Bottom Left

	{ vec3(SKYBOX_DIST, -SKYBOX_DIST, SKYBOX_DIST),
	vec4(0), vec2(1.0f, 1.0f) }, //Bottom Right

	{ vec3(SKYBOX_DIST, SKYBOX_DIST, SKYBOX_DIST),
	vec4(0), vec2(1.0f, 0.0f) },// Top Right

	//Back
	{ vec3(-SKYBOX_DIST, SKYBOX_DIST, -SKYBOX_DIST),
	vec4(0), vec2(0.0f, 0.0f) },// Top Left

	{ vec3(-SKYBOX_DIST, -SKYBOX_DIST, -SKYBOX_DIST),
	vec4(0), vec2(0.0f, 1.0f) },// Bottom Left

	{ vec3(SKYBOX_DIST, -SKYBOX_DIST, -SKYBOX_DIST),
	vec4(0), vec2(1.0f, 1.0f) }, //Bottom Right

	{ vec3(SKYBOX_DIST, SKYBOX_DIST, -SKYBOX_DIST),
	vec4(0), vec2(1.0f, 0.0f) },// Top Right
};

vector<int> skyBoxIndices = {
	//front
	3, 2, 1,
	3, 1, 0,

	//left
	0, 1, 5,
	0, 5, 4,

	//right
	7, 6, 2,
	7, 2, 3,

	//bottom
	5, 1, 2,
	5, 2, 6,

	//top
	0, 4, 7,
	0, 7, 3,

	//back
	4, 5, 6,
	4, 6, 7
};

Game::Game()
{
	_window = nullptr;
	_currentGameState = GameState::PLAY;
	_paused = false;
	_gGameController = nullptr;
}

Game::~Game()
{
}

int Game::Run()
{
	Input::LoadInput();
	//performing initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "ERROR SDL_Init" << SDL_GetError() << endl;
		return -1;
	}

	int imageInitFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	int returnInitFlags = IMG_Init(imageInitFlags);
	if ((returnInitFlags & imageInitFlags) != imageInitFlags)
	{
		cout << "Error initializing SDL_Image " << IMG_GetError() << endl;
		return -1;
	}

	if (TTF_Init() == -1)
	{
		printf("TTF Init error: %s\n", TTF_GetError());
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	_window = SDL_CreateWindow("GP3-GAME", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H,
		SDL_WINDOW_OPENGL);
	_glContext = SDL_GL_CreateContext(_window);
	CHECK_GL_ERROR();
	Graphics::Init();
	Graphics::SetViewport(SCREEN_W, SCREEN_H);
	CHECK_GL_ERROR();

	if (SDL_SetRelativeMouseMode(SDL_TRUE) != 0)
		printf("Failed to set mouse mode to Relative\n");

	LoadResources();
	CHECK_GL_ERROR();

	//Check for joysticks
	if (SDL_NumJoysticks() < 1)
	{
		printf("Warning: No joysticks connected!\n");
	}
	else
	{
		//Load joystick
		_gGameController = SDL_GameControllerOpen(0);
		if (_gGameController == NULL)
		{
			printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
		}
	}

	Loop();
}

void Game::Loop()
{

	Uint32 oldTicks = 0;
	SDL_Event event;

	while (_currentGameState != GameState::EXIT)
	{

		SDL_GameControllerUpdate();

		inputTicks = SDL_GetTicks();
		Input::Update();
		Input::CleanInputActions();
		Input::CleanInputAxis();

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				_currentGameState = GameState::EXIT;
				break;
			case SDL_WINDOWEVENT_CLOSE:
				_currentGameState = GameState::EXIT;
				break;
			case SDL_KEYDOWN:
				Input::SetKey(event.key.keysym.sym, true);
				Input::SetMod(event.key.keysym.mod);
				break;
			case SDL_KEYUP:
				Input::SetKey(event.key.keysym.sym, false);
				Input::SetMod(event.key.keysym.mod);
				break;
			case SDL_MOUSEMOTION:
				Input::MoveMouse(ivec2(event.motion.xrel, event.motion.yrel));
				break;
			case SDL_CONTROLLERBUTTONDOWN:
				Input::SetGamepadButton(event.cbutton.button, true);
				break;
			case SDL_CONTROLLERBUTTONUP:
				Input::SetGamepadButton(event.cbutton.button, false);
				break;
			case SDL_CONTROLLERAXISMOTION:
				Input::SetGamepadAxis(event.caxis.axis, event.caxis.value);
				break;
			}
		}
		Input::UpdateInputActions();
		Input::UpdateInputAxis();

		Uint32 ticks = SDL_GetTicks();

		inputTime = ticks - inputTicks;
		float deltaTime = (ticks - oldTicks) / 1000.f;
		oldTicks = ticks;

		if (Input::GetKey(SDLK_ESCAPE))
			_currentGameState = GameState::EXIT;

		if (Input::GetKeyDown(SDLK_SPACE)) 
		{
			if (_currentGameState == GameState::PLAY) _currentGameState = GameState::PAUSE;
			else _currentGameState = GameState::PLAY;
		}

		if (_currentGameState == GameState::PAUSE)
			continue; // restart loop if paused to prevent new game state updates and renders but still allow input

		Update(deltaTime);
		updateTicks = SDL_GetTicks();
		updateTime = updateTicks - ticks;
		CHECK_GL_ERROR();
		Render(deltaTime);
		renderTime = SDL_GetTicks() - updateTicks;
		CHECK_GL_ERROR();

		SDL_GL_SwapWindow(_window);
		CHECK_GL_ERROR();
	}

	//Close game controller
	SDL_GameControllerClose(_gGameController);
	_gGameController = NULL;

	ReleaseResources();
	SDL_GL_DeleteContext(_glContext);
	SDL_DestroyWindow(_window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void Game::LoadResources()
{
	resourceManager = new ResourceManager();
	currentScene = new Scene(resourceManager);
	sceneManager = new SceneManager(resourceManager);

	DefRenderer::Init();
	PostProcessing::Init();
	CHECK_GL_ERROR();

	//======================== TEXTURES ========================
	//HIDE THIS AWAY IN TO SCENE MANAGER LOADER AT SOME POINT!
	Texture* skyTexture = new Texture(TEXTURE_PATH + "right.jpg", TEXTURE_PATH + "left.jpg", TEXTURE_PATH + "top.jpg", TEXTURE_PATH + "bottom.jpg", TEXTURE_PATH + "back.jpg", TEXTURE_PATH + "front.jpg");
	resourceManager->AddTexture("skyTexture", skyTexture);
	Texture* skyNightTexture = new Texture(TEXTURE_PATH + "night_right.jpg", TEXTURE_PATH + "night_left.jpg", TEXTURE_PATH + "night_top.jpg", TEXTURE_PATH + "night_bottom.jpg", TEXTURE_PATH + "night_back.jpg", TEXTURE_PATH + "night_front.jpg");
	resourceManager->AddTexture("skyNightTexture", skyNightTexture);

	//========================  MODELS  ========================
	Model *terrainModel = new Model();
	terrainModel->SetUpAttrib(0, 3, GL_FLOAT, 0); //pos
	terrainModel->SetUpAttrib(1, 4, GL_FLOAT, sizeof(vec3)); //coeffs
	terrainModel->SetUpAttrib(2, 2, GL_FLOAT, sizeof(vec3) + sizeof(vec4)); //uv
	terrainModel->SetUpAttrib(3, 3, GL_FLOAT, sizeof(vec3) + sizeof(vec4) + sizeof(vec2)); //normals
	resourceManager->AddModel("Terrain", terrainModel);

	Model *skyModel = new Model();
	vector<Vertex> *verts = new vector<Vertex>(skyBoxverts);
	vector<int> *indcs = new vector<int>(skyBoxIndices);
	skyModel->SetVertices(verts, GL_STATIC_DRAW, true);
	skyModel->SetIndices(indcs, GL_STATIC_DRAW, true);
	skyModel->FlushBuffers();
	skyModel->SetUpAttrib(0, 3, GL_FLOAT, 0);
	resourceManager->AddModel("skyModel", skyModel);

	//======================== SCENEMANAGEMENT  ====================
	sceneManager->LoadSceneDirectories();
	sceneManager->LoadScene(0, currentScene);
	currentScene->Begin();
}

void Game::ReleaseResources()
{
	resourceManager->ReleaseResources();
	PostProcessing::CleanUp();
}

void Game::Update(float deltaTime)
{
	currentScene->Update(deltaTime);
	DefRenderer::SetMainLightDir(dynamic_cast<Light*>(currentScene->GetMainDirLight()->GetComponent("Light"))->GetLightDirection());
	DefRenderer::SetMainLightColor(vec3(dynamic_cast<Light*>(currentScene->GetMainDirLight()->GetComponent("Light"))->GetColor()));
	  
	if (Input::GetKeyDown(SDLK_f))
	{
		debugMode = !debugMode;
		if (debugMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (Input::GetKeyDown(SDLK_b)) 
		negMode = !negMode;

	fpsCounter++;
	if ((fpsTimer += deltaTime) > 1)
	{
		fpsDisplay = fpsCounter;
		fpsCounter = fpsTimer = 0;
	}

	char fpsBuffer[30];
	sprintf_s(fpsBuffer, "FPS: %d", fpsDisplay);
	string fpsString(fpsBuffer);
	resourceManager->GetFont("OratorStd.otf")->Render(fpsString, { 0, 0, 100, 25 });

	char performanceBuffer[90];
	sprintf_s(performanceBuffer, "Input: %dms, Update: %dms, Render: %dms", inputTime, updateTime, renderTime);
	string performanceString(performanceBuffer);
	resourceManager->GetFont("OratorStd.otf")->Render(performanceString, { 0, (int)SCREEN_H - 25, 325, 25 });
}

bool Comparer(GameObject *a, GameObject *b)
{
	if (a->GetComponent("SkyBox")) //skybox should be rendered last
		return false;
	if (b->GetComponent("SkyBox")) //so that depth testing has bigger impact
		return true;

	Renderer *aRenderer = a->GetRenderer();
	Renderer *bRenderer = b->GetRenderer();
	if (!aRenderer) //all gameobjects without renderer go last
		return false;
	if (!bRenderer)
		return true;

	//first, sort by VAOs
	Model *aModel = aRenderer->GetModel();
	Model *bModel = bRenderer->GetModel();
	if (!aModel)
		return false;
	if (!bModel)
		return true;
	GLuint aVao = aModel->Get();
	GLuint bVao = bModel->Get();

	if (aVao == bVao)
		return aRenderer->GetProgram() > bRenderer->GetProgram();
	else
		return aVao > bVao;
}

void Game::Render(float deltaTime)
{
 	drawCalls = verticesRendered = 0;

	Camera *camera = currentScene->GetSceneCamera();
	camera->Recalculate();
	currentScene->VisibilityCheck();
	currentScene->Sort(Comparer);

	currentScene->RenderDirShadowMap();

	if (debugMode)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		currentScene->Render(camera);
	}
	else
	{
		DefRenderer::BeginGeomGather();
		currentScene->Render(camera);
		DefRenderer::EndGeomGather();

		DefRenderer::BeginLightGather();
		int count = currentScene->GetLightCount();
		for (int i = 0; i < count; i++)
		{
			Renderer *r = currentScene->GetLight(i);
			DefRenderer::StencilPass(camera, r);
			DefRenderer::LightPass(camera, r);
		}
		DefRenderer::EndLightGather();

		glBindFramebuffer(GL_FRAMEBUFFER, PostProcessing::Get());

		DefRenderer::RenderGather();

		if(negMode)
			PostProcessing::Pass(resourceManager->GetShader("PostProcess1"));
		PostProcessing::RenderResult();
	}

	char msg[50];
	sprintf_s(msg, "verts:%u", verticesRendered);
	Font* tF = resourceManager->GetFont("OratorStd.otf");
	tF->Render(string(msg), { 0, 25, 100, 25 });
	sprintf_s(msg, "objts:%u(%u)", currentScene->GetVisibleGOCount(), currentScene->GetGOCount());
	tF->Render(string(msg), { 0, 50, 100, 25 });

	resourceManager->FlushFonts(deltaTime);
}