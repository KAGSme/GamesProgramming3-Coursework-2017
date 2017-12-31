#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "Common.h"

struct Material
{
public:
	vec4 diffuseColor;
	float specPower;
	float specIntensity;

	Material() 
	{
		specPower = 32;
		diffuseColor = vec4(0.8, 0.8, 0.8, 0.8);
		specIntensity = 0.5;
	}
};

#endif // !_MATERIAL_H