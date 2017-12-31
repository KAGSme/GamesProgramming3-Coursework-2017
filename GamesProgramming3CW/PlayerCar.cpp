#include "PlayerCar.h"
#include "Input.h"



void PlayerCar::strafe(float state)
{
	if(!debug)
	strafeAccel = state * Game::GetGlobalDeltaTime() * 1000;
}

PlayerCar::PlayerCar()
{
	velocity = vec3(0, 0, 0);
	friction = 10;
	maxStrafeVel = 2;
	leftBound = -40; //keep negative as it's to the left of player (in world)
	rightBound = 40; //keep positive as it's to the right of player (in world)

	GameObject* camGO;
	camGO = Game::GetCurrentScene()->AddGameObject("FollowCamFirst", vec3(0), vec3(0, -90, 0), vec3(1));
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
	InputAction* iaF = Input::GetInputActionState("Debug");
	if (iaF)
		__unhook(&InputAction::InputActionChange, iaF, &PlayerCar::FreeCam);

}

void PlayerCar::OnBegin()
{
	//model for player car is rotated so we rotate it and set up a new right direction
	newRight = -pGameObject->GetTransform()->GetUp();
	InputAxis* iaH = Input::GetInputAxisState("HorizontalCar");
	if (iaH)
		__hook(&InputAxis::InputAxisChange, iaH, &PlayerCar::strafe);

	InputAction* iaS = Input::GetInputActionState("SwitchCam");
	if (iaS)
		__hook(&InputAction::InputActionChange, iaS, &PlayerCar::switchCam);

	InputAction* iaF = Input::GetInputActionState("Debug");
	if (iaF)
		__hook(&InputAction::InputActionChange, iaF, &PlayerCar::FreeCam);

	pGameObject->AttachCollider(new SphereCol(3.0f));
	Game::GetResourceManager()->GetSound("carEngine.wav")->playAudio(AL_NONE);
}

void PlayerCar::Update(float deltaTime)
{
	if (pGameObject->GetActive())
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

		_timeScore += deltaTime * 6;
		_finalScore = int(_timeScore) * 50 + _otherScore;

		Sound* sound = Game::GetResourceManager()->GetSound("carEngine.wav");
		if (sound->GetState() == AL_STOPPED)
			sound->playAudio(AL_NONE);
	}
}

void PlayerCar::OnRender(Camera *camera)
{
	string score = "Score: " + to_string(_finalScore);
	int scoreLength = score.size();
	Game::GetResourceManager()->GetFont("Regensburg.ttf")->Render("Health: " + to_string(health), { 0, (int)SCREEN_H - 30, 100, 25 });
	Game::GetResourceManager()->GetFont("Regensburg.ttf")->Render(score, { 0, (int)SCREEN_H - 60, static_cast<__int64>(scoreLength) * 12, 25 });
}

void PlayerCar::AddHealth(int amount)
{
	health += amount;

	if (health <= 0)
	{
		cout << "Player Death" << endl;
		Game::GetResourceManager()->GetSound("carEngine.wav")->stopAudio();
		Game::GetResourceManager()->GetSound("Death.wav")->playAudio(AL_NONE);
		Game::GetResourceManager()->GetSound("end-game-fail.wav")->playAudio(AL_NONE);
		pGameObject->SetActive(false);
	}
}

void PlayerCar::Reset()
{
}

void PlayerCar::switchCam(bool state)
{
	if (state && pGameObject->GetActive() && !debug)
	{
		firstPerson = !firstPerson;
		if (firstPerson)
		{
			_FirstPersonCam->SetSceneTargetCameraToThis();
		}
		else _ThirdPersonCam->SetSceneTargetCameraToThis();
	}
}

void PlayerCar::FreeCam(bool state)
{
	if (state)
	{
		debug = !debug;
		if (debug)
		{
			GameObject* go = Game::GetCurrentScene()->GetGameObject("CameraBehaviourObject");
			if (go)
			{
				dynamic_cast<CameraComponent*>(go->GetComponent("CameraComponent"))->SetSceneTargetCameraToThis();
			}
		}
		else switchCam(true);
	}
}
