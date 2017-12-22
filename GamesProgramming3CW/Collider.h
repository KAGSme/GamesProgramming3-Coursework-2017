#ifndef _COLLIDER_H
#define _COLLIDER_H

struct SphereCol 
{
private:
	vec3 position;
public:
	float radius = 0;

	void update(vec3 newPosition) { position = newPosition; };

	bool isOverlap(SphereCol* other)
	{
		vec3 delta = position - other->position;
		float distance = delta.length();
		
		if (distance < radius + other->radius)
		{
			return true;
		}
		else return false;
	}
	__event void onOverLap(GameObject* owner);
};

#endif // !_COLLIDERS_H

