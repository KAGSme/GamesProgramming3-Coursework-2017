#ifndef _CAMERACOMPONENT_H
#define _CAMERACOMPONENT_H
#include "BaseComponent.h"
#include "Camera.h"
#include "GameObject.h"
#include "Game.h"

class CameraComponent :public BaseComponent
{
private:
	Camera *_camera;

public:

	void OnBegin();
	void Update(float DeltaTime);
	void SetSceneTargetCameraToThis();

	void OnOverlap(GameObject * other);
};

#endif