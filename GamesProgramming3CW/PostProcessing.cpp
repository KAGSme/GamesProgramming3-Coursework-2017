#include "PostProcessing.h"
#include "Graphics.h"

GLuint PostProcessing::fboDepthBuffer[2];
GLuint PostProcessing::fbo[2];
Model *PostProcessing::model;
ShaderProgram *PostProcessing::program;
Renderer *PostProcessing::renderer;
Texture *PostProcessing::texture[2];
int PostProcessing::activeInd;

void PostProcessing::Init()
{
	activeInd = 0;
	ivec2 screen = Graphics::GetViewport();

	//creating all the required objects
	//creating the 2 frame buffers - needed to be able to swap between 2 during post processing passes

	glGenFramebuffers(2, fbo);
	GLuint FBOtexture[2]; //texture for our frame buffer
	glGenTextures(2, FBOtexture);
	glGenRenderbuffers(2, fboDepthBuffer); //and the depth buffers
	CHECK_GL_ERROR();

	//creating required resources for 2 fbos
	for (int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo[i]);
		glBindTexture(GL_TEXTURE_2D, FBOtexture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, screen.x, screen.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		texture[i] = new Texture(FBOtexture[i]);
		CHECK_GL_ERROR();

		glBindRenderbuffer(GL_RENDERBUFFER, fboDepthBuffer[i]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, screen.x, screen.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fboDepthBuffer[i]);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		CHECK_GL_ERROR();

		//configuring frame buffer
		//setting FBOtexture as color attachment 0
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOtexture[i], 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fboDepthBuffer[i]);
		CHECK_GL_ERROR();

		//marking that frag shader will render to the bound buffer
		GLenum bufferToDraw[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, bufferToDraw);
		CHECK_GL_ERROR();

		//check if we succeeded
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			printf("Error - Framebuffer incomplete!\n");
	}

	//now create all the required resources for rendering the screen quad
	vector<Vertex> *verts = new vector<Vertex>();
	verts->push_back(Vertex(vec3(-1.f, -1.f, 0)));
	verts->push_back(Vertex(vec3(1.f, -1.f, 0)));
	verts->push_back(Vertex(vec3(-1.f, 1.f, 0)));
	verts->push_back(Vertex(vec3(1.f, 1.f, 0)));

	model = new Model();
	model->SetVertices(verts, GL_STATIC_DRAW, true);
	model->FlushBuffers();
	model->SetUpAttrib(0, 2, GL_FLOAT, 0); //vec2 position

	program = new ShaderProgram(SHADER_PATH + "postProcVS.glsl", SHADER_PATH + "postProcFS.glsl");
	program->BindAttribLoc(0, "vertexPosition");
	program->Link();

	renderer = new Renderer();
	renderer->AddTexture(texture[0]);
	renderer->SetModel(model, GL_TRIANGLE_FAN);
	renderer->SetShaderProgram(program);
}

void PostProcessing::CleanUp()
{
	glDeleteFramebuffers(2, fbo);
	glDeleteRenderbuffers(2, fboDepthBuffer);
	delete program;
	delete model;
	delete texture[0];
	delete texture[1];
	delete renderer;
}

void PostProcessing::Pass(ShaderProgram *newProgram)
{
	renderer->SetShaderProgram(newProgram);
	//binding a populated texture
	renderer->SetTexture(0, texture[activeInd]);
	//swapping so that we render from populated to clean
	activeInd = ++activeInd % 2;
	glBindFramebuffer(GL_FRAMEBUFFER, fbo[activeInd]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer->Ready();
	renderer->Render();

	//restore the shader
	renderer->SetShaderProgram(program);
}

void PostProcessing::RenderResult()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer->SetTexture(0, texture[activeInd]);
	renderer->Ready();
	renderer->Render();
}