#include "CameraComponent.h"

void CameraComponent::OnBegin()
{
	SetSceneTargetCameraToThis();
}

void CameraComponent::Update(float DeltaTime)
{
}

void CameraComponent::SetSceneTargetCameraToThis()
{
	_camera = Game::currentScene->GetSceneCamera();
	_camera->SetParentTransform(pGameObject->GetTransform());
}
