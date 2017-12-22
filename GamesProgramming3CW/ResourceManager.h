#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include "common.h"
#include "GameObject.h"
#include "Font.h"
#include <map>

class ResourceManager 
{
private:
map<string, ShaderProgram*> shaderPrograms;
map<string, Texture*> textures;
map<string, Model*> models;
map<string, Font*> fonts;

Texture* currentTexture;
Shader* currentShader;
Model* currentModel;

public:
	ResourceManager();
	~ResourceManager();
//adds shader to the map, should be done at the start of application
void AddShader(ShaderProgram* shader, string name);
//adds model to the model map
void AddModel(const string& filename);
void AddModel(const string& name, Model *m) { models[name] = m; }
//adds texture to the map
void AddTexture(const string& filename);
void AddTexture(const string& name, Texture* t) { textures[name] = t; }
//adds fonts to the map
void AddFont(const string& filename);
void AddFont(const string& name, Font* f) { fonts[name] = f; }

ShaderProgram* GetShader(const string& name);
Texture* GetTexture(const string& name);
Model* GetModel(const string& name);
Font* GetFont(const string& name);

void FlushFonts(float deltaTime);
//Deletes all the things!
void ReleaseResources();
};
#endif
