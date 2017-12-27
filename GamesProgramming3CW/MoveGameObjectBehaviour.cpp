#include "MoveGameObjectBehaviour.h"
#include "input.h"
#include "GameObject.h"

MoveGameObjectBehaviour::MoveGameObjectBehaviour()
{

}


MoveGameObjectBehaviour::~MoveGameObjectBehaviour()
{

}

void MoveGameObjectBehaviour::OnBegin()
{
	pGameObject->AttachCollider(new SphereCol(100.0f));
}

void MoveGameObjectBehaviour::Update(float deltaTime)
{
	if (Input::GetKey(SDLK_r))
		GetParentGO()->GetTransform()->AddRotationEuler(vec3(30 * deltaTime, 0, 0));
	if (Input::GetKey(SDLK_t))
		GetParentGO()->GetTransform()->AddRotationEuler(vec3(-30 * deltaTime, 0, 0));
	if (Input::GetKey(SDLK_z))
		GetParentGO()->GetTransform()->AddRotationEuler(vec3(0, 0, 30 * deltaTime));
	if (Input::GetKey(SDLK_x))
		GetParentGO()->GetTransform()->AddRotationEuler(vec3(0, 0, -30 * deltaTime));
	if (Input::GetKey(SDLK_c))
		GetParentGO()->GetTransform()->AddRotationEuler(vec3(0, 30 * deltaTime, 0));
	if (Input::GetKey(SDLK_v))
		GetParentGO()->GetTransform()->AddRotationEuler(vec3(0, -30 * deltaTime, 0));
	if (Input::GetKey(SDLK_o))
		GetParentGO()->GetTransform()->AddPosition(vec3(30 * deltaTime, 0, 0));
	if (Input::GetKey(SDLK_p))
		GetParentGO()->GetTransform()->AddPosition(vec3(-30 * deltaTime, 0, 0));
	if (Input::GetKey(SDLK_k))
		GetParentGO()->GetTransform()->AddPosition(vec3(0,0,30 * deltaTime));
	if (Input::GetKey(SDLK_l))
		GetParentGO()->GetTransform()->AddPosition(vec3(0,0,-30 * deltaTime));
	if (Input::GetKey(SDLK_n))
		GetParentGO()->GetTransform()->AddPosition(vec3( 0, 30 * deltaTime, 0));
	if (Input::GetKey(SDLK_m))
		pGameObject->Destroy();
		//GetParentGO()->GetTransform()->AddPosition(vec3( 0, -30 * deltaTime, 0)); 

}


