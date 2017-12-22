#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::AddShader(ShaderProgram * shader, string name)
{
	shaderPrograms[name] = shader;
}

void ResourceManager::AddModel(const string & filename)
{
	if(models.find(filename) == models.end())
		models[filename] = new Model(MODEL_PATH + filename);
}

void ResourceManager::AddTexture(const string & filename)
{
	textures[filename] = new Texture(TEXTURE_PATH + filename);
}

void ResourceManager::AddFont(const string & filename)
{
	fonts[filename] = new Font(FONT_PATH + filename);
}

ShaderProgram* ResourceManager::GetShader(const string& name)
{
	return shaderPrograms[name];
}

Texture* ResourceManager::GetTexture(const string& name)
{
	return textures[name];
}

Model* ResourceManager::GetModel(const string& name)
{
	return models[name];
}

Font * ResourceManager::GetFont(const string & name)
{
	return fonts[name];
}

void ResourceManager::FlushFonts(float deltaTime)
{
	for (auto iter = fonts.begin(); iter != fonts.end(); iter++)
		iter->second->Flush(deltaTime);
}

void ResourceManager::ReleaseResources()
{
	for (auto iter = textures.begin(); iter != textures.end(); iter++)
		delete iter->second;

	for (auto iter = models.begin(); iter != models.end(); iter++)
		delete iter->second;

	for (auto iter = fonts.begin(); iter != fonts.end(); iter++)
		delete iter->second;
}
