#ifndef _VERTEX_H
#define _VERTEX_H

#include "Common.h"

struct Vertex
{
	vec3 pos = vec3(0, 0, 0);
	vec4 color = vec4(0, 0, 0, 0);
	vec2 texture = vec2(0, 0);
	vec3 normal = vec3(0, 0, 0);
	vec3 tangent = vec3(0, 0, 0);
	vec3 binormal = vec3(0, 0, 0);

	Vertex() {}

	Vertex(vec3 p)
	{
		pos = p;
	}

	Vertex(vec3 p, vec4 c) : Vertex(p)
	{
		color = c;
	}

	Vertex(vec3 p, vec4 c, vec2 t) : Vertex(p, c)
	{
		texture = t;
	}

	Vertex(vec3 p, vec4 c, vec2 t, vec3 n) : Vertex(p, c, t)
	{
		normal = n;
	}
};

#endif