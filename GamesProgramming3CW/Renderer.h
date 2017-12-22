#ifndef _RENDERER_H
#define _RENDERER_H

#include "Common.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "Texture.h"
#include "BaseComponent.h"
#include "Camera.h"

#define TEXTURE_COUNT 5

class Renderer: public BaseComponent
{
private:
	Texture* textures[TEXTURE_COUNT];
	ShaderProgram *shaderProg = NULL;
	Model *model = NULL;
	int renderMode = 0;
	int textCount = 0;
	bool transparent = false;
	GLuint matVbo = 0;
	bool instancedRender = false;
	uint instanceCount = 0;

	static GLuint activeProg, activeVao;
	static GLuint activeTexts[TEXTURE_COUNT];
	bool isCubeMap = false;

public:
	Renderer() { }
	Renderer(Texture *t, bool cubeMap, ShaderProgram *s, Model *m, int mode);
	~Renderer() {}

	//mode supports either GL_TRIANGLE_FAN or GL_TRIANGLES
	void SetModel(Model *pModel, int mode) { model = pModel; renderMode = mode; }
	Model* GetModel() { return model; }

	void SetShaderProgram(ShaderProgram *sP) { shaderProg = sP; }
	ShaderProgram* GetProgram() { return shaderProg; }

	void AddTexture(Texture *t, bool cubeMap = false) { textures[textCount++] = t; isCubeMap = cubeMap; }
	void SetTexture(int i, Texture *t, bool cubeMap = false) { textures[i] = t; isCubeMap = cubeMap; }
	Texture* GetTexture(int i) { return textures[i]; }
	int GetTextureCount() { return textCount; }

	void SetTransparent(bool flag) { transparent = flag; }
	//if the object is transparent. used to render in separate queue
	bool GetTransparent() { return transparent; }

	//binds the shader so that uniforms can be sent
	void Ready();
	//call Ready() before this!
	void Render();
	//NYI! Call Ready() before this!
	void RenderInstanced();
	void SetInstanceMatrices(vector<mat4> *matrices);
};

#endif