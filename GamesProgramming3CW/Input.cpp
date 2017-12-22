#include "Input.h"

Uint16 Input::mod;
char Input::keys[400];
char Input::keysOld[400];
ivec2 Input::mPos, Input::mPosOld;
Uint32 Input::mState, Input::mStateOld;
char Input::gamePadButtons[20];
char Input::gamePadButtonsOld[20];
int Input::_axis[10];
vector<InputAction> Input::_inputActions;
vector<InputAxis> Input::_inputAxis;

//Raw Input buttons-------------------------------------------------------------------------

void Input::SetKey(SDL_Keycode key, bool state)
{
	if (key >= SDLK_CAPSLOCK)
	{
		keys[key - (1 << 30) + 128] = state;
		//printf("%d - %c %d\n", state, key, key - (1 << 30) + 128);
	}
	else
	{
		keys[key] = state;
		//printf("%d - %c %d\n", state, key, key);
	}
}

bool Input::GetKey(SDL_Keycode key)
{
	if (key >= SDLK_CAPSLOCK)
		return keys[key - (1 << 30) + 128];
	else
		return keys[key];
}

bool Input::GetKeyDown(SDL_Keycode key)
{
	if (key >= SDLK_CAPSLOCK)
		return keys[key - (1 << 30) + 128] && !keysOld[key - (1 << 30) + 128];
	else
		return keys[key] && !keysOld[key];
}

bool Input::GetKeyUp(SDL_Keycode key)
{
	if (key >= SDLK_CAPSLOCK)
		return !keys[key - (1 << 30) + 128] && keysOld[key - (1 << 30) + 128];
	else
		return !keys[key] && keysOld[key];
}

void Input::SetGamepadButton(Uint8 button, bool state)
{
	gamePadButtons[button] = state;
}

bool Input::GetGamepadButton(Uint8 button)
{
	return gamePadButtons[button];
}

bool Input::GetGamepadButtonDown(Uint8 button)
{
	return gamePadButtons[button] && !gamePadButtonsOld[button];
}

bool Input::GetGamepadButtonUp(Uint8 button)
{
	return !gamePadButtons[button] && gamePadButtonsOld[button];
}

//Load Input Actions and Axis---------------------------------------------------------

XMLError Input::LoadInput()
{
	XMLError eResult;
	string fileName = GAMEDATA_PATH + "input.xml";
	XMLDocument *gamedataXML = new XMLDocument();

	CommonXml::LoadXML(fileName, gamedataXML);

	XMLNode *IAroot = gamedataXML->FirstChild();
	if (!IAroot)
		return XML_ERROR_FILE_READ_ERROR;

	//load Input Actions--------------------------------------------

	XMLElement *actionItems = IAroot->FirstChildElement("Actions");
	if(!actionItems)
		return  XML_ERROR_PARSING_ELEMENT;

	XMLElement *actionItem = actionItems->FirstChildElement("IA");
	while (actionItem)
	{
		InputAction* tempInputAction = new InputAction;

		tempInputAction->name = actionItem->Attribute("title");

		string temp = actionItem->Attribute("keyboard");
		vector<string> attributeInputs;

		attributeInputs = split(temp, ',');
		for (int i = 0 ; i < attributeInputs.size(); i++) 
		{
			tempInputAction->keys.push_back(*attributeInputs.at(i).c_str());
		}

		temp = actionItem->Attribute("gamepad");
		attributeInputs = split(temp, ',');

		for (int i = 0; i < attributeInputs.size(); i++)
		{
			tempInputAction->gamepadButtons.push_back
				(CommonXml::GetGamepadButtonFromInputXml(attributeInputs.at(i)));
		}

		_inputActions.push_back(*tempInputAction);
		tempInputAction = new InputAction;
		actionItem = actionItem->NextSiblingElement("IA");
	}

	//load input Axis----------------------------------------

	XMLElement *axisItems = actionItems->NextSiblingElement("Axis");
	if (!axisItems)
		return XML_ERROR_PARSING;

	XMLElement *axisItem = axisItems->FirstChildElement("IA");
	while (axisItem)
	{
		InputAxis* tempInputAxis = new InputAxis;

		tempInputAxis->name = axisItem->Attribute("title");

		string temp = axisItem->Attribute("keyboardPositive");
		vector<string> attributeInputs;

		attributeInputs = split(temp, ',');
		for (int i = 0; i < attributeInputs.size(); i++)
		{
			tempInputAxis->keysPos.push_back(*attributeInputs.at(i).c_str());
		}

		temp = axisItem->Attribute("keyboardNegative");

		attributeInputs = split(temp, ',');
		for (int i = 0; i < attributeInputs.size(); i++)
		{
			tempInputAxis->keysNeg.push_back(*attributeInputs.at(i).c_str());
		}

		temp = axisItem->Attribute("gamepadPositive");

		attributeInputs = split(temp, ',');
		for (int i = 0; i < attributeInputs.size(); i++)
		{
			tempInputAxis->gamepadButtonsPos.push_back
			(CommonXml::GetGamepadButtonFromInputXml(attributeInputs.at(i)));
		}

		temp = axisItem->Attribute("gamepadNegative");

		attributeInputs = split(temp, ',');
		for (int i = 0; i < attributeInputs.size(); i++)
		{
			tempInputAxis->gamepadButtonsNeg.push_back
			(CommonXml::GetGamepadButtonFromInputXml(attributeInputs.at(i)));
		}

		temp = axisItem->Attribute("gamepadAnalog");

		attributeInputs = split(temp, ',');
		for (int i = 0; i < attributeInputs.size(); i++)
		{
			tempInputAxis->gamepadAxis.push_back
			(CommonXml::GetGamepadAxisFromInputXml(attributeInputs.at(i)));
		}

		_inputAxis.push_back(*tempInputAxis);
		tempInputAxis = new InputAxis;
		axisItem = axisItem->NextSiblingElement("IA");
	}

	delete gamedataXML;
	return XML_SUCCESS;
	return tinyxml2::XMLError();
}

bool Input::GetInputAction(string button)
{
	for (auto &iter : _inputActions)
	{
		if (iter.name == button)
			return iter.state;
	}

	return false;
}

bool Input::GetInputActionDown(string button)
{
	for (auto &iter : _inputActions)
	{
		if (iter.name == button)
			return iter.state && !iter.stateOld;
	}

	return false;
}

bool Input::GetInputActionUp(string button)
{
	for (auto &iter : _inputActions)
	{
		if (iter.name == button)
			return !iter.state && iter.stateOld;
	}

	return false;
}

InputAction* Input::GetInputActionState(string button)
{
	for (auto &iter : _inputActions)
	{
		if (iter.name == button)
			return &iter;
	}

	return nullptr;
}

void Input::UpdateInputActions()
{
	for (auto &iter : _inputActions)
	{
		for (auto &gamePadButton : iter.gamepadButtons)
		{
			if(GetGamepadButton(gamePadButton)) iter.state = true;
		}

		for (auto &key : iter.keys)
		{
			if (GetKey(key)) iter.state = true;
		}
		if (iter.state != iter.stateOld)
			iter.InputActionChange(iter.state);
	}
}

void Input::CleanInputActions()
{
	for (auto &iter : _inputActions)
	{
		iter.stateOld = iter.state;
		iter.state = false;
	}
}

void Input::UpdateInputAxis()
{
	for (auto &iter : _inputAxis)
	{
		float Pos = 0.0f;
		float Neg = 0.0f;

		for (auto &key : iter.keysPos)
		{
			if (GetKey(key)) Pos = 1;
		}

		for (auto &key : iter.keysNeg)
		{
			if (GetKey(key)) Neg = -1;
		}

		for (auto &gamepadButton : iter.gamepadButtonsPos)
		{
			if (GetGamepadButton(gamepadButton)) Pos = 1;
		}

		for (auto &gamepadButton : iter.gamepadButtonsNeg)
		{
			if (GetGamepadButton(gamepadButton)) Neg = -1;
		}

		for (auto &gamePadAxis : iter.gamepadAxis)
		{
			if (GetGamepadAxis(gamePadAxis) > Pos)Pos = GetGamepadAxis(gamePadAxis);
			else if (GetGamepadAxis(gamePadAxis) < Neg) Neg = GetGamepadAxis(gamePadAxis);
		}

		iter.state = Pos + Neg;
		if (iter.state != 0)
		{
			iter.InputAxisChange(iter.state);
		}
	}
}

float Input::GetInputAxis(string name)
{
	for (auto &iter : _inputAxis)
	{
		if (iter.name == name)
			return iter.state;
	}
	return 0;
}

InputAxis* Input::GetInputAxisState(string name)
{
	for (auto &iter : _inputAxis)
	{
		if (iter.name == name)
			return &iter;
	}

	return nullptr;
}

void Input::CleanInputAxis()
{
	for (auto &iter : _inputAxis)
	{
		iter.state = 0.0f;
	}
}

//Raw Input Axis---------------------------------------------------------------------

void Input::SetGamepadAxis(Uint8 axis, int value)
{
	if (axis == SDL_CONTROLLER_AXIS_LEFTY || axis == SDL_CONTROLLER_AXIS_RIGHTY)
		_axis[axis] = -1 * value;
	else _axis[axis] = value;
}

float Input::GetGamepadAxis(Uint8 axis)
{
	return (float)_axis[axis] / 32767.0f;
}

//Raw Mouse Button-------------------------------------------------------------------

bool Input::GetMouseBtn(int btn)
{
	return mState & SDL_BUTTON(btn);
}

bool Input::GetMouseBtnDown(int btn)
{
	bool newState = mState & SDL_BUTTON(btn);
	bool oldState = mStateOld & SDL_BUTTON(btn);
	return newState && !oldState;
}

bool Input::GetMouseBtnUp(int btn)
{
	bool newState = mState & SDL_BUTTON(btn);
	bool oldState = mStateOld & SDL_BUTTON(btn);
	return !newState && oldState;
}

//Sets up input for next cycle-------------------------------------------------------

void Input::Update()
{
	memcpy(keysOld, keys, sizeof(keys));
	memcpy(gamePadButtonsOld, gamePadButtons, sizeof(gamePadButtons));
	mStateOld = mState;
	mPosOld = mPos;
	mState = SDL_GetMouseState(NULL, NULL);
}