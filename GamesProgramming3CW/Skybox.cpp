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
	renderer->SetTexture(0, skybox);
	if (isTimeDay)
		renderer->SetTexture(1, skyNight);
	renderer->SetModel(model, GL_TRIANGLES);
	renderer->SetShaderProgram(shader);
	renderer->SetCubeMapMode(true);
}

void Skybox::OnBegin()
{
	tD->SetTimeScale(120);
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
		vec3 dayColor(1.f, 1.f, 0.984f); //slight white
		vec3 nightColor(0.137f, 0.152f, 0.301f); //slight blue
		vec3 dayambient(0.65f, 0.65f, 0.7f);
		vec3 nightambient(0.05f, 0.05f, 0.05f);
		vec3 dayFog(0.415f, 0.670f, 0.764f);
		vec3 nightFog(0,0,0);

		if (nightTime)
		{ 
			Game::GetCurrentScene()->GetMainDirLight()->GetLight()->SetColor(vec4(mix(nightColor, dayColor, k), 1));
			DefRenderer::SetFogColor(mix(nightFog, dayFog, k));
			DefRenderer::SetAmbientColor(mix(nightambient, dayambient, k));
		}
		else
		{ 
			Game::GetCurrentScene()->GetMainDirLight()->GetLight()->SetColor(vec4(mix(dayColor, nightColor, k), 1));
			DefRenderer::SetFogColor(mix(dayFog, nightFog, k));
			DefRenderer::SetAmbientColor(mix(dayambient, nightambient, k));
		}
	}
}

void Skybox::OnRender(Camera *camera)
{
	ShaderProgram *program = renderer->GetProgram();

	mat4 mvp = camera->GetProj() * mat4(mat3(camera->GetView()));
	program->SetUniform("VP", value_ptr(mvp));
	program->SetUniform("blendFactor", &blendFactor);
}