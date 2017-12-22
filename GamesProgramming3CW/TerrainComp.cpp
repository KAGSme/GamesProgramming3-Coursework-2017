#include "TerrainComp.h"
#include "GameObject.h"
#include "Game.h"

TerrainComp::TerrainComp(const string &fileName, vec3 size)
{
	//parsing the heightmap
	vertices = new vector<Vertex>();
	indices = new vector<int>();

	SDL_Surface *surf = IMG_Load(fileName.c_str());
	if (!surf)
		printf("Couldn't load image %s-%s\n", fileName.c_str(), IMG_GetError());
	width = surf->w;
	float wStep = size.x / width;
	height = surf->h;
	float hStep = size.z / height;
	
	//first, creating the vertices
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			vec3 pixel = GetPixel(surf, x, y);
			Vertex v;
			v.pos.x = x * wStep;
			v.pos.z = y * hStep;
			v.pos.y = size.y * pixel.r;
			v.texture.x = x % 2;
			v.texture.y = y % 2;
			v.color = GetHeightRanges(pixel.r);
			vertices->push_back(v);
		}
	}

	//now creating indices
	for (int y = 0; y < height - 1; y++)
	{
		for (int x = 0; x < width - 1; x++)
		{
			int tl = y * width + x;
			int tr = tl + 1;
			int bl = tl + width;
			int br = bl + 1;
			indices->push_back(tl);
			indices->push_back(bl);
			indices->push_back(tr);
			indices->push_back(br);
			indices->push_back(tr);
			indices->push_back(bl);
		}
	}
	SDL_FreeSurface(surf);
}

TerrainComp::~TerrainComp()
{
}

void TerrainComp::SetParentGO(GameObject *pGO)
{
	pGameObject = pGO;

	//attaching the mesh generated based on a heightmap
	Renderer *renderer = (Renderer*)pGameObject->GetComponent("Renderer");
	Model *model = renderer->GetModel();
	model->SetVertices(vertices, GL_STATIC_DRAW, true);
	model->SetIndices(indices, GL_STATIC_DRAW, true);
	model->Normalize();
	model->FlushBuffers();

	//next up, tree population
	//setting up the object for instanced tree rendering
	ResourceManager *mngr = Game::resourceManager;
	Scene *scene = Game::currentScene;
	Model *tree = mngr->GetModel("Tree.fbx");
	tree->SetBoundSphereUse(false);
	ShaderProgram *prog = mngr->GetShader("Tree");
	Texture *bark = mngr->GetTexture("TreeBark.png");
	Texture *leaves = mngr->GetTexture("TreeLeaves.png");
	GameObject *treesParent = new GameObject();
	Renderer *treesRenderer = new Renderer();
	treesRenderer->SetModel(tree, GL_TRIANGLES);
	treesRenderer->SetShaderProgram(prog);
	treesRenderer->AddTexture(bark);
	treesRenderer->AddTexture(bark);
	treesRenderer->AddTexture(leaves);
	treesRenderer->SetTransparent(true);
	
	//create all the MVP matrices
	const int treeCount = 100;
	vector<mat4> modelMats;
	srand(time(NULL));
	for (int i = 0; i < treeCount; i++)
	{
		int x = rand() % width;
		int z = rand() % height;
		int y = vertices->at(z * width + x).pos.y;

		mat4 model = translate(mat4(1), vec3(x, y, z));
		model = rotate(model, radians(-90.f), vec3(1, 0, 0));
		modelMats.push_back(model);
	}

	//push it in to the scene
	treesRenderer->SetInstanceMatrices(&modelMats);
	treesParent->AttachComponent(treesRenderer);
	scene->AddGameObject(treesParent);

	//rock population, 5 variances
	prog = mngr->GetShader("Rock");
	int instCount = 500;
	vector<string> mNames = { "Rock1.fbx", "Rock2.fbx", "Rock3.fbx", "Rock4.fbx", "Rock5.fbx" };
	vector<string> tNames = { "Rock1.jpg", "Rock2.jpg", "Rock3.jpg", "Rock4.jpg", "Rock5.jpg" };
	for (int i = 0; i < mNames.size(); i++)
	{
		Model *rock = mngr->GetModel(mNames[i]);
		rock->SetBoundSphereUse(false);
		Texture *text = mngr->GetTexture(tNames[i]);
		GameObject *go = new GameObject();
		Renderer *r = new Renderer();
		r->SetModel(rock, GL_TRIANGLES);
		r->SetShaderProgram(prog);
		r->AddTexture(text);

		modelMats.clear();
		for (int j = 0; j < instCount; j++)
		{
			int x = rand() % width;
			int z = rand() % height;
			int y = vertices->at(z * width + x).pos.y;
			float randScale = (float)(rand() % 10) / 10 + 0.5f;

			mat4 model = translate(mat4(1), vec3(x, y, z));
			model = scale(model, vec3(randScale));
			modelMats.push_back(model);
		}
		r->SetInstanceMatrices(&modelMats);
		go->AttachComponent(r);
		scene->AddGameObject(go);
	}

	//ferns now
	instCount = 500;
	mNames = { "Fern1.fbx", "Fern2.fbx" };
	tNames = { "Fern1.png", "Fern2.png" };
	for (int i = 0; i < mNames.size(); i++)
	{
		Model *rock = mngr->GetModel(mNames[i]);
		rock->SetBoundSphereUse(false);
		Texture *text = mngr->GetTexture(tNames[i]);
		GameObject *go = new GameObject();
		Renderer *r = new Renderer();
		r->SetModel(rock, GL_TRIANGLES);
		r->SetShaderProgram(prog);
		r->AddTexture(text);
		r->SetTransparent(true);

		modelMats.clear();
		for (int j = 0; j < instCount; j++)
		{
			int x = rand() % width;
			int z = rand() % height;
			int y = vertices->at(z * width + x).pos.y;
			float randScale = (float)(rand() % 5) / 100 + 0.05;

			mat4 model = translate(mat4(1), vec3(x, y, z));
			model = scale(model, vec3(randScale));
			modelMats.push_back(model);
		}
		r->SetInstanceMatrices(&modelMats);
		go->AttachComponent(r);
		scene->AddGameObject(go);
	}
}

//http://stackoverflow.com/questions/17270538/how-to-change-rgb-values-in-sdl-surface
vec3 TerrainComp::GetPixel(SDL_Surface *s, int x, int y)
{
	uint32 pixel = *(uint32*)((uint8 *)s->pixels + y * s->pitch + x * s->format->BytesPerPixel);

	uint8 r, g, b;
	SDL_GetRGB(pixel, s->format, &r, &g, &b);
	return vec3(r / 255.f, g / 255.f, b / 255.f);
}

vec4 TerrainComp::GetHeightRanges(float height)
{
	//defines the first and 3rd height ranges {0, T, 1-T, 0}
	const float T = 0.4f;
	//subrages around T and 1-T points, for blending
	const float EPSILON = 0.1f;

	float k0 = 1 - height / (T + EPSILON);
	k0 = clamp(k0, 0.f, 1.f);
	float k1 = 1 - abs(height - 0.5) / (0.5 - T + EPSILON);
	k1 = clamp(k1, 0.f, 1.f);
	float k2 = (height - (1 - T - EPSILON)) / (T + EPSILON);
	k2 = clamp(k2, 0.f, 1.f);

	return vec4(k0, k1, k2, 0);
}