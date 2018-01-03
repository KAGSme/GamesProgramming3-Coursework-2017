#include "Common.h"

void InterpVec3To(vec3 &vecStart, vec3 destination, float speed, float deltaTime)
{
	//if distance is below speed, just set the vector to the destination
	if (length(destination - vecStart) <= speed * deltaTime)
	{
		vecStart = destination;
		return;
	}

	vec3 dir = normalize(destination - vecStart);
	vecStart += dir * speed * deltaTime;
}