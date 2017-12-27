#ifndef _DEF_RENDERer_H
#define _DEF_RENDERer_H

#include "Common.h"
#include "Renderer.h"
#include "Camera.h"
#include "Game.h"

class DefRenderer
{
private:
	static GLuint fbo;
	static GLuint rbo; //holds the depth and stencil

	//keeping track of model and shader
	static Texture *textures[4];
	static Model *model;
	static ShaderProgram *program;
	static ShaderProgram *nullProg;
	static Renderer *renderer;

	static vec3 mainLightDir;
	static vec3 mainLightColor;
	static vec3 ambientLightColor;
	static vec3 fogColor;
	static float fogMaxDistance;

	DefRenderer() {}
public:
	~DefRenderer() {}

	static void Init();
	static void CleanUp();
	//get the deferred renderer frame buffer
	static GLuint Get() { return fbo; }
	static Texture* GetFrameTexture(int i) { return textures[i]; };
	static void BeginGeomGather();
	static void EndGeomGather();
	static void BeginLightGather();
	static void EndLightGather();
	static void StencilPass(Camera *cam, Renderer *r);
	static void LightPass(Camera *cam, Renderer *r);
	static void RenderGather();

	static vec3 GetMainLightDir() { return mainLightDir; }
	static void SetMainLightDir(vec3 dir) { mainLightDir = dir; }

	static vec3 GetSunColor() { return mainLightColor; }
	static void SetMainLightColor(vec3 color) { mainLightColor = color; }

	static void SetFogColor(vec3 color) { fogColor = color; }
	static void SetAmbientColor(vec3 color) { ambientLightColor = color; }

};

#endif