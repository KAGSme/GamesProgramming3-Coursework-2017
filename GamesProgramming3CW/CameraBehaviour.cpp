#include "CameraBehaviour.h"
#include "Input.h"
#include "GameObject.h"
#include "Game.h"

float baseSpeed;
float boostSpeed;
CameraBehaviour::CameraBehaviour(Camera *c, float tS)
{
	camera = c;
	speed = tS;
	baseSpeed = speed;
	boostSpeed = baseSpeed * 4;
	mouseSensitivity = 0.2f;
	aimSensitivity = 30;
}

CameraBehaviour::~CameraBehaviour()
{
	InputAxis* iaV = Input::GetInputAxisState("Vertical");
	if (iaV)
		__unhook(&InputAxis::InputAxisChange, iaV, &CameraBehaviour::InputAxisVertical);

	InputAxis* iaH = Input::GetInputAxisState("Horizontal");
	if (iaH)
		__unhook(&InputAxis::InputAxisChange, iaH, &CameraBehaviour::InputAxisHorizontal);

	InputAction* iaS = Input::GetInputActionState("SpeedBoostToggle");
	if(iaS)
		__unhook(&InputAction::InputActionChange, iaS, &CameraBehaviour::InputSpeedBoost);

	InputAction* iaS2 = Input::GetInputActionState("SpeedBoost");
	if (iaS)
		__unhook(&InputAction::InputActionChange, iaS, &CameraBehaviour::InputSpeedBoostHold);
}

void CameraBehaviour::OnBegin()
{
	InputAxis* iaV = Input::GetInputAxisState("Vertical");
	if (iaV)
		__hook(&InputAxis::InputAxisChange, iaV, &CameraBehaviour::InputAxisVertical);

	InputAxis* iaH = Input::GetInputAxisState("Horizontal");
	if (iaH)
		__hook(&InputAxis::InputAxisChange, iaH, &CameraBehaviour::InputAxisHorizontal);

	InputAction* iaS = Input::GetInputActionState("SpeedBoostToggle");
	if (iaS)
		__hook(&InputAction::InputActionChange, iaS, &CameraBehaviour::InputSpeedBoost);

	InputAction* iaS2 = Input::GetInputActionState("SpeedBoost");
	if (iaS2)
		__hook(&InputAction::InputActionChange, iaS2, &CameraBehaviour::InputSpeedBoostHold);
}

void CameraBehaviour::Update(float deltaTime)
{
	vec3 up = camera->GetUp();

	//Handle Player Input-------------------------------------------------------------------------------------------

	float hover = Input::GetInputAxis("Hover");
	float aimVertical = Input::GetInputAxis("AimVertical");
	float aimHorizontal = Input::GetInputAxis("AimHorizontal");

	if (hover > 0.2f || hover < -0.2f)
		pGameObject->GetTransform()->AddPosition(up * deltaTime * speed * hover);

	//get mouse movement
	vec2 deltaPos = Input::GetMouseDelta();
	if (aimVertical > 0.2f || aimVertical < -0.2f || aimHorizontal > 0.2f || aimHorizontal < -0.2f)
		deltaPos = vec2(aimHorizontal * aimSensitivity, aimVertical * aimSensitivity);
	
	float deltaPitch = (float)deltaPos.y * mouseSensitivity;
	float deltaYaw = (float)deltaPos.x * mouseSensitivity;

	quat qPitch = quat(vec3(radians(deltaPitch), 0, 0));
	quat qYaw = quat(vec3(0, radians(deltaYaw), 0));

	pGameObject->GetTransform()->GetTransformationMatrix();
	
	//combine rotations this way to eliminate unwanted roll
	quat total = qPitch * pGameObject->GetTransform()->GetRotation() * qYaw;
	pGameObject->GetTransform()->SetRotation(total);

	vec3 gRot = pGameObject->GetTransform()->GetRotationEuler();

	//char buffer[50];
	//sprintf_s(buffer, "Camera Rotation: %2.2f %2.2f %2.2f", gRot.x, gRot.y, gRot.z);
	//Game::GetResourceManager()->GetFont("OratorStd.otf")->Render(string(buffer), { 0, (int)SCREEN_H - 90, 325, 25 });
}

void CameraBehaviour::InputAxisVertical(float state)
{
	pGameObject->GetTransform()->AddPosition(camera->GetForward() * Game::GetGlobalDeltaTime() * speed * state);
}

void CameraBehaviour::InputAxisHorizontal(float state)
{
	pGameObject->GetTransform()->AddPosition(camera->GetRight() * Game::GetGlobalDeltaTime() * speed * state);
}

void CameraBehaviour::InputSpeedBoost(bool state)
{
	if (state)
	{
		boostToggle = !boostToggle;

		if (boostToggle)
			speed = boostSpeed;
		else speed = baseSpeed;
	}
}

void CameraBehaviour::InputSpeedBoostHold(bool state)
{
	if (!boostToggle) 
	{
		if (state) speed = boostSpeed;
		else speed = baseSpeed;
	}
}
