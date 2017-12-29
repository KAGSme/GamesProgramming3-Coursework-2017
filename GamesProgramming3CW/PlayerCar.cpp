#include "PlayerCar.h"
#include "Input.h"



void PlayerCar::strafe(float state)
{
	strafeAccel = state * Game::GetGlobalDeltaTime() * 1000;
	cout << pGameObject->GetTransform()->GetPosition().z << endl;
}

PlayerCar::PlayerCar()
{
	velocity = vec3(0, 0, 0);
	friction = 10;
	maxStrafeVel = 2;
	leftBound = -30; //keep negative as it's to the left of player (in world)
	rightBound = 30; //keep positive as it's to the right of player (in world)

	GameObject* camGO;
	camGO = Game::GetCurrentScene()->AddGameObject("FollowCamFirsty", vec3(0), vec3(0, -90, 0), vec3(1));
	_FirstPersonCam = new CameraComponent();
	camGO->AttachComponent(_FirstPersonCam);
	camGO->AttachComponent(new FollowComponent(vec3(-4, 1, 0), "PlayerCar"));

	camGO = Game::GetCurrentScene()->AddGameObject("FollowCamThird", vec3(0), vec3(0, -90, 0), vec3(1));
	_ThirdPersonCam = new CameraComponent();
	camGO->AttachComponent(_ThirdPersonCam);
	camGO->AttachComponent(new FollowComponent(vec3(30, 10, 0), "PlayerCar"));
}


PlayerCar::~PlayerCar()
{
	InputAxis* iaH = Input::GetInputAxisState("HorizontalCar");
	if (iaH)
		__unhook(&InputAxis::InputAxisChange, iaH, &PlayerCar::strafe);
	InputAction* iaS = Input::GetInputActionState("SwitchCam");
	if (iaH)
		__unhook(&InputAction::InputActionChange, iaS, &PlayerCar::switchCam);
}

void PlayerCar::OnBegin()
{
	newRight = -pGameObject->GetTransform()->GetUp();
	InputAxis* iaH = Input::GetInputAxisState("HorizontalCar");
	if (iaH)
		__hook(&InputAxis::InputAxisChange, iaH, &PlayerCar::strafe);

	InputAction* iaS = Input::GetInputActionState("SwitchCam");
	if (iaH)
		__hook(&InputAction::InputActionChange, iaS, &PlayerCar::switchCam);

	pGameObject->AttachCollider(new SphereCol(3.0f));
}

void PlayerCar::Update(float deltaTime)
{
	if (isActive)
	{
		accel += strafeAccel * newRight * deltaTime;
		velocity += accel;
		velocity.x = clamp(velocity.x, -maxStrafeVel, maxStrafeVel);
		velocity.z = clamp(velocity.z, -maxStrafeVel, maxStrafeVel);

		//cout << velocity.x << " " << velocity.z << endl;

		pGameObject->GetTransform()->AddPosition(velocity);

		InterpVec3To(velocity, vec3(0, 0, 0), friction, deltaTime); 
		accel = vec3(0);
		strafeAccel = 0;

		float radius = pGameObject->GetCollider()->radius;
		pGameObject->GetTransform()->SetPosition(vec3(
			pGameObject->GetTransform()->GetPosition().x, 
			pGameObject->GetTransform()->GetPosition().y, 
			clamp(pGameObject->GetTransform()->GetPosition().z,leftBound+radius,rightBound-radius)));
	}
}

void PlayerCar::AddHealth(int amount)
{
	health += amount;

	if (health <= 0) cout << "Player Death" << endl;
}

void PlayerCar::switchCam(bool state)
{
	if (state)
	{
		firstPerson = !firstPerson;
		if (firstPerson)
		{
			_FirstPersonCam->SetSceneTargetCameraToThis();
		}
		else _ThirdPersonCam->SetSceneTargetCameraToThis();
	}
}
