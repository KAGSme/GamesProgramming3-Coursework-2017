#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_ttf.h>

//maths
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

const string ASSET_PATH = "assets/";
const string SHADER_PATH = ASSET_PATH + "shaders/";
const string TEXTURE_PATH = ASSET_PATH + "textures/";
const string FONT_PATH = ASSET_PATH + "fonts/";
const string MODEL_PATH = ASSET_PATH + "models/";
const string GAMEDATA_PATH = ASSET_PATH + "gamedata/";

void checkError(const char *file, int line);

#ifdef _DEBUG
#define CHECK_GL_ERROR() checkError(__FILE__, __LINE__)
#else
#define CHECK_GL_ERROR()
#endif

void printVec3(vec3 p);
void printRect(SDL_Rect r);

vector<string> split(const string& s, const char delim);

struct RECTF
{
	float x, y;
	float w, h;
};

#define SCREEN_W 1280.f
#define SCREEN_H 768.f

void InterpVec3To(vec3 &vecStart, vec3 destination, float speed, float deltaTime);

#endif