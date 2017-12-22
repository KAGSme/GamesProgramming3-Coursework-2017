#ifndef _POST_PROCESSING
#define _POST_PROCESSING

#include "Common.h"
#include "Renderer.h"

//class which is used to apply postprocessing to the
//current frame
class PostProcessing
{
private:
	static GLuint fboDepthBuffer[2];
	static GLuint fbo[2];
	static Model *model;
	static ShaderProgram *program;
	static Renderer *renderer;
	static Texture *texture[2];
	static int activeInd;

	PostProcessing() {}
public:
	~PostProcessing() {}

	static void Init();
	static GLuint Get() { return fbo[activeInd]; }

	//allows applying simple shaders (which use 1 texture)
	static void Pass(ShaderProgram *newProgram);
	static void RenderResult();
	static void CleanUp();
};

#endif