#include "Camera.h"

Camera::Camera(bool orthoMode)
{
	yaw = -90; 
	pitch = 0;
	pos = vec3(0, 0, 10);
	up = vec3(0, 1, 0);

	//setting up the matrix for UI rendering
	//call Recalculate to get proper perspective matrix
	this->orthoMode = orthoMode;
	if (orthoMode)
		SetProjOrtho(0, SCREEN_W, 0, SCREEN_H);
	else
		SetProjPersp(45, SCREEN_W / SCREEN_H, 0.1f, 1000.f);
}

Camera::~Camera()
{
}

void Camera::Recalculate()
{
	if (_parentTransform != nullptr)
	{
		_parentTransform->GetTransformationMatrix();

		forward = -1.f * _parentTransform->GetForward();
		right = _parentTransform->GetRight();
		up = _parentTransform->GetUp();

		pos = _parentTransform->GetPosition();

		frustrum.UpdateFrustrum(pos, right, up, forward);
		viewMatrix = lookAt(pos, pos + forward, up);

		VP = projMatrix * viewMatrix;
	}
	else 
	{
		quat qPitch = angleAxis(radians(pitch), vec3(1, 0, 0));
		quat qYaw = angleAxis(radians(yaw), vec3(0, 1, 0));
		quat qRoll = angleAxis(radians(roll), vec3(0, 0, 1));

		quat orientation = qPitch * qYaw;
		orientation = normalize(orientation);
		mat4 currentRotate = mat4_cast(orientation);

		mat4 currentTranslate = mat4(1.0f);
		currentTranslate = translate(currentTranslate, -pos);

		viewMatrix = currentRotate * currentTranslate;

		forward = -1.f * vec3(0, 0, 1) * mat3(currentRotate);
		up = vec3(0, 1, 0) * mat3(currentRotate);
		right = vec3(1, 0, 0) * mat3(currentRotate);

		frustrum.UpdateFrustrum(pos, right, up, forward);
		VP = projMatrix * viewMatrix;
	}
}

void Camera::RotateTo(vec3 target)
{
	vec3 newForward = normalize(target - pos);
	vec3 newRight = normalize(cross(vec3(0, 1, 0), -newForward));
	vec3 newUp = cross(-newForward, right);

	pitch = degrees(asin(-newForward.y));
	yaw = degrees(atan2(newForward.y, newForward.x));

}

void Camera::SetProjPersp(float fov, float ratio, float nearPlane, float farPlane)
{
	orthoMode = false;
	
	projMatrix = perspective(fov, ratio, nearPlane, farPlane);
	frustrum.SetFrustrumDef(fov, ratio, nearPlane, farPlane);
}

void Camera::SetProjOrtho(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
	orthoMode = true;

	projMatrix = ortho(left, right, bottom, top, nearPlane, farPlane);
	VP = projMatrix;
}

void Camera::SetProjOrtho(float left, float right, float bottom, float top)
{
	orthoMode = true;

	projMatrix = ortho(left, right, bottom, top);
	VP = projMatrix;
}

