#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const string& vShaderFile, const string& fShaderFile)
{
	vShader = new Shader(vShaderFile, VERTEX_SHADER);
	fShader = new Shader(fShaderFile, FRAGMENT_SHADER);
	CHECK_GL_ERROR();

	program = glCreateProgram();
	glAttachShader(program, vShader->Get());
	glAttachShader(program, fShader->Get());
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(program);
}

void ShaderProgram::BindAttribLoc(GLuint loc, const char *name)
{
	glBindAttribLocation(program, loc, name);
	CHECK_GL_ERROR();
}

void ShaderProgram::Link()
{
	//link shaders and attribs
	glLinkProgram(program);
	CheckForLinkErrors();
	CHECK_GL_ERROR();

	//if they were linked we no longer need them
	//delete vShader;
	//delete fShader;

	//going to iterate through every uniform and cache info about it
	GLint uniformCount = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniformCount);
	//printf("Contains %d uniforms:\n", uniformCount);
	const int maxLength = 100;
	char nameChars[maxLength];
	for (int i = 0; i < uniformCount; i++)
	{
		GLenum type = 0;
		GLsizei nameLength = 0;
		GLsizei uniSize = 0;
		glGetActiveUniform(program, i, maxLength, &nameLength, &uniSize, &type, nameChars);
		string name(nameChars, nameLength);
		GLint loc = glGetUniformLocation(program, name.c_str());
		locations[name] = loc;
		types[name] = type;

		//printf("\t%d(%d). %s - %d\n", i, loc, name.c_str(), type);
	}
}

void ShaderProgram::SetUniform(const string& name, void *value)
{
	switch (types[name])
	{
	case GL_SAMPLER_CUBE:
	case GL_SAMPLER_2D:
	case GL_INT:
		glUniform1i(locations[name], *(GLint*)value);
		break;
	case GL_FLOAT:
		glUniform1f(locations[name], *(GLfloat*)value);
		break;
	case GL_FLOAT_VEC2:
		glUniform2f(locations[name], ((vec2*)value)->x, ((vec2*)value)->y);
		break;
	case GL_FLOAT_VEC3:
		glUniform3f(locations[name], ((vec3*)value)->x, ((vec3*)value)->y, ((vec3*)value)->z);
		break;
	case GL_FLOAT_VEC4:
		glUniform4f(locations[name], ((vec4*)value)->x, ((vec4*)value)->y, ((vec4*)value)->z, ((vec4*)value)->w);
		break;
	case GL_FLOAT_MAT4:
		glUniformMatrix4fv(locations[name], 1, false, (const GLfloat*)value);
		break;
	}
}

bool ShaderProgram::CheckForLinkErrors()
{
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

	if (isLinked == GL_FALSE)
	{
		GLint length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

		char *msg = (char*)malloc(length);
		glGetProgramInfoLog(program, length, &length, msg);

		printf("Shader linking error: %s\n", msg);
		free(msg);

		glDeleteProgram(program);
		return true;
	}
	return false;
}