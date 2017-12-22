#ifndef _INPUT_ACTIONS_H
#define _INPUT_ACTIONS_H

#include "Common.h"

struct InputAction
{
	string name = "";
	char state = false;
	char stateOld = true;
	vector<Uint8> gamepadButtons;
	vector<SDL_Keycode> keys;

public:
	__event void InputActionChange(bool state);
};

struct InputAxis
{
	string name = "";
	float state = 0.0f;
	float stateOld;
	vector<Uint8> gamepadAxis;
	vector<SDL_Keycode> keysPos;
	vector<SDL_Keycode> keysNeg;
	vector<Uint8> gamepadButtonsPos;
	vector<Uint8> gamepadButtonsNeg;

public:
	__event void InputAxisChange(float axis);
};

#endif