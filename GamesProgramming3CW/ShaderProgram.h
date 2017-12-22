#ifndef _SHADERPROG_H
#define _SHADERPROG_H

#include "Common.h"
#include "Shader.h"

class ShaderProgram
{
private:
	Shader *vShader, *fShader;
	GLuint program;
	map<string, GLint> locations;
	map<string, GLenum> types;

	bool CheckForLinkErrors();

public:
	ShaderProgram(const string& vShaderFile, const string& fShaderFile);
	~ShaderProgram();

	void BindAttribLoc(GLuint loc, const char *name);
	void Link();
	void SetUniform(const string& name, void *value);

	GLuint Get() { return program; }
};

#endif