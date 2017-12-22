#include "Graphics.h"
#include "Vertices.h"
#include "Common.h"

ivec2 Graphics::viewportRect;

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

void Graphics::Init()
{
	glewExperimental = true;
	GLenum err = glewInit();
	if (err != GLEW_OK)
		cout << "GLEW Error: " << glewGetErrorString(err) << endl;

	//ignoring the first error
	glGetError();

	//Smooth shading
	glShadeModel(GL_SMOOTH);

	//clear with black
	glClearColor(0, 0, 0, 0);

	//clear depth to 1
	glClearDepth(1);

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); //set less or equal func for depth testing
	
	//enabling blending
	//glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//turn on back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Graphics::SetViewport(int width, int height)
{
	if (height == 0)
		height = 1;
	
	//setting up the viewport
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	viewportRect = ivec2(width, height);
}