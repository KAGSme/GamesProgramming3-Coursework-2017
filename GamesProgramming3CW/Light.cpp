#include "Light.h"
#include "Game.h"

Light::Light(vec4 color)
{
	this->color = color;
	_state = E_LightState::POINT;
}

Light::Light(vec4 color, E_LightState state = E_LightState::POINT)
{
	_state = state;
	this->color = color;

	if (_state == E_LightState::DIRECTIONAL)
	{
		glGenFramebuffers(1, &depthMapFBO);

		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);  

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) cout << "FRAMEBUFFER DOWN MATE!" << endl;
		//glBindFramebuffer(GL_FRAMEBUFFER, 0); 

		depthTexture = new Texture(depthMap);
		Game::resourceManager->AddTexture("DirShadowMap", depthTexture);
	}
}

Light::~Light()
{
	glDeleteFramebuffers(1, &depthMapFBO);
	glDeleteTextures(1, &depthMap);
}

Camera * Light::ConfigureDirLightCamera()
{
	Camera *lightCamera = new Camera();
	GLfloat nearPlane = -10, farPlane = 20;
	lightCamera->SetProjOrtho(-100, 100, -100, 100, -10, 1000);
	lightCamera->SetParentTransform(pGameObject->GetTransform());
	lightCamera->Recalculate(); 
	depthVP = lightCamera->Get();

	return lightCamera;  
}

void Light::DirShadowMapRenderStart()
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);  
}

void Light::DirShadowMapRenderEnd()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_W, SCREEN_H);
}

void Light::Update(float deltaTime)
{
	if (_state == E_LightState::DIRECTIONAL)
	{
		direction = pGameObject->GetTransform()->GetForward(); 

		char buffer[50];  
		sprintf_s(buffer, "DIR LIGHT direction: %.2f %.2f %.2f", direction.x, direction.y, direction.z);
		Game::resourceManager->GetFont("OratorStd.otf")->Render(string(buffer), { 0, (int)SCREEN_H - 60, 325, 25 });
	}
}
