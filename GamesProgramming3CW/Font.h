#ifndef _FONT_H
#define _FONT_H

#include "Common.h"
#include "Renderer.h"

class Font
{
private:
	Camera *guiCam;
	//the size of the atlas texture
	int width = 512, height = 512;
	//contains normalized UVs
	RECTF rects[127 - 32];
	//contains vertices for submitting to gpu
	vector<Vertex> *vertices;
	vector<int> *indices;

	Renderer *renderer;
	Model *m;
	Texture *t;
	ShaderProgram *shader;

	float timer = 0;
	bool preRenderStage = false;

	GLuint Font::ConvertSDLSurfaceToTexture(SDL_Surface *surf);

public:
	Font(const string& fileName);
	~Font();

	void Render(const string& text, const SDL_Rect rect);
	void Flush(float deltaTime);
};

#endif