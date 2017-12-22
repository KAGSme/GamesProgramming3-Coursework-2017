#ifndef _TERRAIN_COMP_H
#define _TERRAIN_COMP_H

#include "Common.h"
#include "BaseComponent.h"
#include "Vertex.h"

class TerrainComp : public BaseComponent
{
private:
	vector<Vertex> *vertices;
	vector<int> *indices;
	int width, height;

	//returns normalized vector of RGB color
	vec3 GetPixel(SDL_Surface *s, int x, int y);
	vec4 GetHeightRanges(float height);

public:
	TerrainComp(const string &fileName, vec3 size);
	~TerrainComp();

	void SetParentGO(GameObject *pGO);
};

#endif