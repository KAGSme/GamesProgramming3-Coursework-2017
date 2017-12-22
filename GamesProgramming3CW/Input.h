#ifndef _INPUT_H
#define _INPUT_H

#include "Common.h"
#include "InputActions.h"
#include "tinyxml2.h"
#include "CommonXml.h"

using namespace tinyxml2;

class Input
{
private:
	static Uint16 mod;
	static char keys[400]; //400 because 127 goes for ascii keys,
	static char keysOld[400]; //then ~250 goes o n for other keys
	static ivec2 mPos, mPosOld;
	static Uint32 mState, mStateOld;
	static char gamePadButtons[20];
	static char gamePadButtonsOld[20];
	static int _axis[10];
	static vector<InputAction> _inputActions;
	static vector<InputAxis> _inputAxis;

public:
	static void SetMod(Uint16 pMod) { mod = pMod; }

	static void SetKey(SDL_Keycode key, bool state);
	static bool GetKey(SDL_Keycode key); //check if it's currently down or not
	static bool GetKeyDown(SDL_Keycode key); //check if it was pressed this frame
	static bool GetKeyUp(SDL_Keycode key); //check if it was released this frame

	static void SetGamepadButton(Uint8 button, bool state);
	static bool GetGamepadButton(Uint8 button); //check if it's currently down or not
	static bool GetGamepadButtonDown(Uint8 button); //check if it was pressed this frame
	static bool GetGamepadButtonUp(Uint8 button); //check if it was released this frame

	static XMLError LoadInput(); 

	static bool GetInputAction(string button);
	static bool GetInputActionDown(string button);
	static bool GetInputActionUp(string button);
	static void UpdateInputActions();
	static void CleanInputActions();
	static InputAction* GetInputActionState(string button);

	static void UpdateInputAxis();
	static float GetInputAxis(string name);
	static void CleanInputAxis();
	static InputAxis* GetInputAxisState(string name);

	static void SetGamepadAxis(Uint8 axis, int value);
	static float GetGamepadAxis(Uint8 axis);

	static ivec2 GetMousePos() { return mPos; }
	static ivec2 GetMouseDelta() { return mPos - mPosOld; }
	static void MoveMouse(ivec2 delta) { mPos += delta; }

	static bool GetMouseBtn(int btn);
	static bool GetMouseBtnDown(int btn);
	static bool GetMouseBtnUp(int btn);

	static void Update();
};

#endif
