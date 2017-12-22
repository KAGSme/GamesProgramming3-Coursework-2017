#include "Transform.h"
#include <glm/gtx/norm.hpp>

mat4 Transform::UpdateTransformMatrix()
{
	_transformMatrix = translate(mat4(1.f), _pos);
	mat4 currentRotate = mat4_cast(_rotation);
	_transformMatrix = _transformMatrix * currentRotate;
	_transformMatrix = scale(_transformMatrix, _scale);

	_forward = vec3(0, 0, 1) * mat3(currentRotate);
	_up = vec3(0, 1, 0) * mat3(currentRotate);
	_right = vec3(1, 0, 0) * mat3(currentRotate);

	_isDirty = false;
	return _transformMatrix;
}

vec3 Transform::GetRotationEuler()
{
	float yaw, pitch, roll;
	float test = _rotation.x * _rotation.y + _rotation.z *_rotation.w;
	if (test > 0.499)
	{
		yaw = 2 * atan2(_rotation.x, _rotation.w);
		roll = M_PI/2;
		pitch = 0;
		return degrees(vec3(pitch, yaw, roll));
	}
	if (test < -0.499)
	{
		yaw = -2 * atan2(_rotation.x, _rotation.w);
		roll = -M_PI / 2;
		pitch = 0;
		return degrees(vec3(pitch, yaw, roll));
	}
	float sqx = _rotation.x * _rotation.x;
	float sqy = _rotation.y * _rotation.y;
	float sqz = _rotation.z * _rotation.z;

	yaw = atan2(2 * _rotation.y* _rotation.w - 2 * _rotation.x * _rotation.z, 1 - 2 * sqy - 2 * sqz);
	roll = asin(2 * test);
	pitch = atan2(2 * _rotation.x*_rotation.w - 2 * _rotation.y*_rotation.z, 1 - 2 * sqx - 2 * sqz);

	return degrees(vec3(pitch, yaw, roll));
}

void Transform::SetRotationEuler(vec3 newRotation)
{
	quat qPitch = angleAxis(radians(newRotation.x), vec3(1, 0, 0));
	quat qYaw = angleAxis(radians(newRotation.y), vec3(0, 1, 0));
	quat qRoll = angleAxis(radians(newRotation.z), vec3(0, 0, 1));
	_rotation = qPitch * qYaw * qRoll;
	_rotation = normalize(_rotation);

	_isDirty = true;
}

void Transform::AddRotationEuler(vec3 deltaRotation)
{
	quat delta = quat(radians(deltaRotation));
	_rotation = delta * _rotation;
	_rotation = normalize(_rotation);

	_isDirty = true;
}

void Transform::AddRotation(quat deltaRotation)
{
	_rotation = deltaRotation * _rotation;

	_isDirty = true;
}

quat Transform::RotationBetweenVectors(vec3 start, vec3 dest) {
	start = normalize(start);
	dest = normalize(dest);

	float cosTheta = dot(start, dest);
	vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f) {
		// special case when vectors in opposite directions:
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);
		if (length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
			rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);

		rotationAxis = normalize(rotationAxis);
		return angleAxis(glm::radians(180.0f), rotationAxis);
	}

	rotationAxis = cross(start, dest);

	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
		);

}


