#ifndef _CAMERABEHAVIOUR_H
#define _CAMERABEHAVIOUR_H
#include "Common.h"
#include "Camera.h"
#include "BaseComponent.h"
class CameraBehaviour: public BaseComponent
{
private:
	Camera *camera;
	bool wireframeMode = false;
	bool boostToggle = false;
public:
	float mouseSensitivity;
	float aimSensitivity;
	float speed;
	CameraBehaviour(Camera * c, float tS);
	~CameraBehaviour();
	void OnBegin();
	void Update(float deltaTime);

	void InputAxisVertical(float state);
	void InputAxisHorizontal(float state);
	void InputSpeedBoost(bool state);
	void InputSpeedBoostHold(bool state);
};
#endif