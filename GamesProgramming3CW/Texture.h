#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "Common.h"

class Texture
{
private:
	GLuint textureId;

	GLuint LoadTextureFromFile(const string& fileName);
	GLuint LoadCubmapFromFiles(
		const string& right, 
		const string& left, 
		const string& top, 
		const string& bottom, 
		const string& back, 
		const string& front);
	GLuint ConvertSDLSurfaceToTexture(SDL_Surface *surf);
	void ConvertSDLSurfaceToCubeMap(SDL_Surface *surf, int i, GLuint textId);

public:
	Texture(GLuint text) { textureId = text; }
	Texture(const string& fileName);
	Texture(
		const string& right,
		const string& left,
		const string& top,
		const string& bottom,
		const string& back,
		const string& front);
	~Texture();

	GLuint Get() { return textureId; }
};

#endif