#include "Shader.h"

Shader::Shader(const string& fileName, SHADER_TYPE type)
{
	shader = LoadShaderFromFile(fileName, type);
	if (CheckForCompileErrors(fileName))
		shader = 0;
	//printf("======================\n");
}

Shader::~Shader()
{
	if (shader != 0)
		glDeleteShader(shader);
}

GLuint Shader::LoadShaderFromMemory(const char *pMem, SHADER_TYPE type)
{
	GLuint shader = glCreateShader(type);
	//printf("Shader:\n%s\n\n", pMem);
	glShaderSource(shader, 1, &pMem, NULL);
	glCompileShader(shader);
	return shader;
}

GLuint Shader::LoadShaderFromFile(const string& fileName, SHADER_TYPE type)
{
	string fileContents;
	ifstream file;
	file.open(fileName.c_str(), std::ios::in);
	if (!file)
	{
		cout << "File could not be loaded" << endl;
		return 0;
	}

	//calculate file size
	if (file.good())
	{
		file.seekg(0, std::ios::end);
		unsigned long len = file.tellg();
		file.seekg(std::ios::beg);
		if (len == 0)
		{
			std::cout << "File has no contents " << std::endl;
			return 0;
		}

		fileContents.resize(len);
		file.read(&fileContents[0], len);
		file.close();
		GLuint program = LoadShaderFromMemory(fileContents.c_str(), type);
		return program;
	}

	return 0;
}

bool Shader::CheckForCompileErrors(const string& fileName)
{
	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint errorLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorLength);

		char *msg = (char*)malloc(errorLength);
		glGetShaderInfoLog(shader, errorLength, &errorLength, msg);

		printf("Error compiling a shader(%s): %s\n", fileName.c_str(), msg);
		free(msg);

		glDeleteShader(shader);
		return true;
	}
	return false;
}