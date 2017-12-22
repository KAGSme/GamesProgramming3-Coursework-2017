#include "CameraComponent.h"

void CameraComponent::OnBegin()
{
	SetSceneTargetCameraToThis();
	pGameObject->AttachCollider(new SphereCol(10.0f));
	__hook(&SphereCol::onOverlap, pGameObject->GetCollider(), &CameraComponent::OnOverlap);
}

void CameraComponent::Update(float DeltaTime)
{
}

void CameraComponent::SetSceneTargetCameraToThis()
{
	_camera = Game::currentScene->GetSceneCamera();
	_camera->SetParentTransform(pGameObject->GetTransform());
}

void CameraComponent::OnOverlap(GameObject * other)
{
	vec3 delta = pGameObject->GetTransform()->GetPosition() - other->GetTransform()->GetPosition();
	float distance = glm::length(delta);
	cout << "Overlapped with " << other->GetName() << " " << distance << " units" << endl;
}


