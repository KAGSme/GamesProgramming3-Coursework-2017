#include "PlayerCar.h"
#include "Input.h"



void PlayerCar::strafe(float state)
{
	strafeAccel = state * Game::GetGlobalDeltaTime() * 1000;
}

PlayerCar::PlayerCar()
{
	velocity = vec3(0, 0, 0);
	friction = 10;
	maxStrafeVel = 2;
}


PlayerCar::~PlayerCar()
{
	InputAxis* iaH = Input::GetInputAxisState("HorizontalCar");
	if (iaH)
		__unhook(&InputAxis::InputAxisChange, iaH, &PlayerCar::strafe);
}

void PlayerCar::OnBegin()
{
	newRight = -pGameObject->GetTransform()->GetUp();
	InputAxis* iaH = Input::GetInputAxisState("HorizontalCar");
	if (iaH)
		__hook(&InputAxis::InputAxisChange, iaH, &PlayerCar::strafe);
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
	}
}
