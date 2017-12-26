#ifndef _TRANSFORM_H
#define _TRANSFORM_H
#include"Common.h"

class Transform
{
private:
	vec3 _pos, _scale, _forward, _right, _up;
	quat _rotation;
	mat4 _transformMatrix;

	bool _isDirty = true;
	mat4 UpdateTransformMatrix();
public:

	vec3 GetPosition() { return _pos; }
	vec3 GetForward() { return _forward; }
	vec3 GetRight() { return _right; }
	vec3 GetUp() { return _up; }
	quat GetRotation() { return _rotation; }
	vec3 GetRotationEuler();
	vec3 GetScale() { return _scale; }
	mat4 GetTransformationMatrix() { if (_isDirty)return UpdateTransformMatrix(); else return _transformMatrix; }

	void SetPosition(vec3 newPos) { _isDirty = true; _pos = newPos; }
	void AddPosition(vec3 deltaPos) { _isDirty = true; _pos += deltaPos; }
	void SetRotation(quat newRotation) { _isDirty = true; _rotation = newRotation; }
	void SetRotationEuler(vec3 newRotation);
	void AddRotationEuler(vec3 deltaRotation);
	void AddRotation(quat deltaRotation);
	void SetScale(vec3 newScale) { _isDirty = true; _scale = newScale; }
	void addScale(vec3 deltaScale) { _isDirty = true; _scale += deltaScale; }

	static quat Transform::RotationBetweenVectors(vec3 start, vec3 dest);
};

#endif