#ifndef _SKYBOX_H
#define _SKYBOX_H

#include "BaseComponent.h"
#include "Renderer.h"
#include "TimeDay.h"

class Skybox : 	public BaseComponent
{
private:
	Renderer *renderer;
	Texture *skybox;
	Texture *skyNight;
	Model *model;
	ShaderProgram *shader;
	TimeDay* tD;
	bool isTimeDay;
	float blendFactor;

public:
	Skybox(Texture *skybox, Model *model, ShaderProgram *shader, Texture *skyNight, bool iTD);
	~Skybox();
	void SetParentGO(GameObject *pGO);

	void Update(float deltaTime);
	void OnRender(Camera *camera);
};

#endif