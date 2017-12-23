#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "Common.h"

struct Material
{
public:
	vec4 diffuseColor;
	float specPower;

	Material() 
	{
		specPower = 80;
		diffuseColor = vec4(0.8, 0.8, 0.8, 0.8);
	}
};

#endif // !_MATERIAL_H