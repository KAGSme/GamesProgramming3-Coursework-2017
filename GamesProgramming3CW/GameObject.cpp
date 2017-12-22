#include "GameObject.h"
#include "Game.h"

GameObject::GameObject()
{
	_transform = new Transform();
}

GameObject::~GameObject()
{
	if (!renderer)
		delete renderer;
}

void GameObject::Begin()
{
	for (auto it = components.begin(); it != components.end(); it++) {
		(*(it->second)).OnBegin();
	}
	
	//add shadow map to renderer
	if (!renderer) return;
	renderer->AddTexture(Game::resourceManager->GetTexture("DirShadowMap"));  
}

void GameObject::Update(float deltaTime)
{
	for (auto it = components.begin(); it != components.end(); it++) {
		(*(it->second)).Update(deltaTime);
	}
}

void GameObject::Render(Camera *camera)
{
	if (!renderer)
		return;

	renderer->Ready();
	ShaderProgram *program = renderer->GetProgram();
	mat4 model = _transform->GetTransformationMatrix();
	program->SetUniform("Model", value_ptr(model));
	mat4 VP = camera->Get();
	program->SetUniform("VP", value_ptr(VP));
	mat4 MVP = VP * model;
	program->SetUniform("MVP", value_ptr(MVP));   
	   
	mat4 shadowDepthMVP = Game::currentScene->GetMainDirLight()->GetLight()->GetShadowMapDepthVP();   
	program->SetUniform("shadowDepthBiasMVP", value_ptr(shadowDepthMVP));

	vec3 camPos = camera->GetPos(); 
	program->SetUniform("cameraPosition", &camPos);

	vec3 lightDir = Game::currentScene->GetMainDirLight()->GetLight()->GetLightDirection();;
	program->SetUniform("lightDirection", &lightDir);
	     
	vec4 ambMatColor(1, 0, 0, 1);
	program->SetUniform("ambientMaterialColor", &ambMatColor);

	vec4 difMatColor(1, 0, 0, 1); 
	program->SetUniform("diffuseMaterialColor", &difMatColor);

	vec4 specMatColor(1, 1, 1, 1);
	program->SetUniform("specularMaterialColor", &specMatColor);

	float power = 25;
	program->SetUniform("specularPower", &power);

	vec4 ambLightColor = Game::currentScene->GetMainDirLight()->GetLight()->GetColor() / 2.f;
	program->SetUniform("ambientLightColor", &ambLightColor); 

	vec4 difLightColor = Game::currentScene->GetMainDirLight()->GetLight()->GetColor();
	program->SetUniform("diffuseLightColor", &difLightColor);

	vec4 specLightColor(1, 1, 1, 1); 
	program->SetUniform("specularLightColor", &specLightColor);

	float heightScale = 5.0f;
	program->SetUniform("heightScale", &heightScale);

	for (auto it = components.begin(); it != components.end(); it++)
		(*(it->second)).OnRender(camera);

	renderer->Render(); 
}

void GameObject::Render(Camera * camera, ShaderProgram* overrideProgram)
{
	if (!renderer)
		return;
	ShaderProgram *program, *oldProgram;
	oldProgram = renderer->GetProgram();
	if (overrideProgram == nullptr)program = renderer->GetProgram();
	else
	{
		program = overrideProgram;
		renderer->SetShaderProgram(program);
	}
	Render(camera);
	if (overrideProgram != nullptr)renderer->SetShaderProgram(oldProgram);
}

void GameObject::AttachComponent(BaseComponent * com)
{
	if (components.count(typeid(*com).name()) >= 1) 
		return;
	
	//caching
	Renderer *r = dynamic_cast<Renderer*>(com);
	if (r)
		renderer = r;

	Light *l = dynamic_cast<Light*>(com);
	if (l)
		light = l;

	components[typeid(*com).name()] = com;
	com->SetParentGO(this);
}

BaseComponent * GameObject::GetComponent(string componentType)
{
	if(components.count("class " + componentType) == 0)
		return NULL;
	else 
		return components.find("class " + componentType)->second;
}

