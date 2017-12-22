#ifndef _SHADER_H
#define _SHADER_H

#include "Common.h"

enum SHADER_TYPE
{
	VERTEX_SHADER = GL_VERTEX_SHADER,
	FRAGMENT_SHADER = GL_FRAGMENT_SHADER
};

class Shader
{
private:
	GLuint shader;

public:
	Shader(const string& fileName, SHADER_TYPE type);
	~Shader();

	GLuint Get() { return shader; }

	GLuint LoadShaderFromMemory(const char *pMem, SHADER_TYPE type);
	GLuint LoadShaderFromFile(const string& fileName, SHADER_TYPE type);
	bool CheckForCompileErrors(const string& fileName);
};

#endif