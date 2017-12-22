#include "Skybox.h"
#include "GameObject.h"
#include "DefRenderer.h"

Skybox::Skybox(Texture *skybox, Model *model, ShaderProgram *shader, Texture *skyN, bool iTD)
{
	this->skybox = skybox;
	this->model = model;
	this->shader = shader;
	isTimeDay = iTD;
	skyNight = isTimeDay ? skyN : skybox;
}

Skybox::~Skybox()
{
}

void Skybox::SetParentGO(GameObject *pGO)
{
	pGameObject = pGO;
	tD = dynamic_cast<TimeDay*>(pGameObject->GetComponent("TimeDay"));
	renderer = pGameObject->GetRenderer();
	renderer->SetTexture(0, skybox, true);
	if (isTimeDay)
		renderer->SetTexture(1, skyNight, true);
	renderer->SetModel(model, GL_TRIANGLES);
	renderer->SetShaderProgram(shader);
}

void Skybox::Update(float deltaTime)
{
	if (isTimeDay)
	{
		bool nightTime = tD->GetHour() < 12;
		float k;
		if (nightTime)
		{
			k = ((tD->GetHour() + tD->GetMinute() / 60.f) / 12.f);
			blendFactor = 1 - k;
		}
		else
		{
			k = ((tD->GetHour() - 12 + tD->GetMinute() / 60.f) / 12.f);
			blendFactor = k;
		}
		
		//first, the sun direction
		float angle;   

		//now, the color
		vec3 dayColor(1.f, 1.f, 1.f); //slight white
		vec3 nightColor(0.113f, 0.039f, 0.380f); //slight blue
		vec3 color;

		if (nightTime)
		{
			angle = mix(0, -90, k);
			color = mix(nightColor, dayColor, k);  
		}
		else
		{ 
			angle = mix(-90, 0, k);
			color = mix(dayColor, nightColor, k);
		}

		//Game::currentScene->GetMainDirLight()->GetLight()->SetColor(vec4(color, 1));
		//Game::currentScene->GetMainDirLight()->GetTransform()->SetRotationEuler(vec3(angle, 0, 0));
	}
}

void Skybox::OnRender(Camera *camera)
{
	ShaderProgram *program = renderer->GetProgram();

	mat4 mvp = camera->GetProj() * mat4(mat3(camera->GetView()));
	program->SetUniform("VP", value_ptr(mvp));
	program->SetUniform("blendFactor", &blendFactor);
}