#ifndef _CUBEMAP_H
#define _CUBEMAP_H

#include "Common.h"

class CubeMap {
private:
	GLuint textureId;

	GLuint LoadTextureFromFile(const string& fileName);
public:
	CubeMap(const string filename);
};

#endif 

