#include "CommonXml.h"

void CommonXml::XmlErrorCheck(XMLError error, string msg)
{
	if (error == XML_SUCCESS)
		cout << "XML: " << msg << " success!" << endl;
	else if (error == XML_ERROR_PARSING_ELEMENT)
		cout << "XML: " << msg << " Parsing Error!" << endl;
	else
		cout << "ERROR XML: " << msg << " failed!" << endl;
}

void CommonXml::LoadXML(string filePath, XMLDocument * xml)
{
	XMLError eResult = xml->LoadFile(filePath.c_str());
	XmlErrorCheck(eResult, "Loading " + filePath);
}

constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

const map<string, Uint8> gamepadStrings =
{
	{ "gamepad_a", SDL_CONTROLLER_BUTTON_A },
	{ "gamepad_b", SDL_CONTROLLER_BUTTON_B },
	{ "gamepad_x", SDL_CONTROLLER_BUTTON_X },
	{ "gamepad_y", SDL_CONTROLLER_BUTTON_Y },
	{ "gamepad_lb", SDL_CONTROLLER_BUTTON_LEFTSHOULDER },
	{ "gamepad_rb", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER },
	{ "gamepad_back", SDL_CONTROLLER_BUTTON_BACK },
	{ "gamepad_start", SDL_CONTROLLER_BUTTON_START },
	{ "gamepad_guide", SDL_CONTROLLER_BUTTON_GUIDE },
	{ "gamepad_ls", SDL_CONTROLLER_BUTTON_LEFTSTICK },
	{ "gamepad_rs", SDL_CONTROLLER_BUTTON_RIGHTSTICK },
	{ "gamepad_up", SDL_CONTROLLER_BUTTON_DPAD_UP },
	{ "gamepad_down", SDL_CONTROLLER_BUTTON_DPAD_DOWN },
	{ "gamepad_left", SDL_CONTROLLER_BUTTON_DPAD_LEFT },
	{ "gamepad_right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT },
	{ "gamepad_LeftStickY", SDL_CONTROLLER_AXIS_LEFTY },
	{ "gamepad_LeftStickX", SDL_CONTROLLER_AXIS_LEFTX },
	{ "gamepad_RightStickY", SDL_CONTROLLER_AXIS_RIGHTY },
	{ "gamepad_RightStickX", SDL_CONTROLLER_AXIS_RIGHTX },
	{ "gamepad_LT", SDL_CONTROLLER_AXIS_TRIGGERLEFT },
	{ "gamepad_RT", SDL_CONTROLLER_AXIS_TRIGGERRIGHT }
};

Uint8 CommonXml::GetGamepadButtonFromInputXml(string xmlInput)
{
	if (gamepadStrings.count(xmlInput) == 0)
		return SDL_CONTROLLER_BUTTON_INVALID;
	else return gamepadStrings.at(xmlInput);
}

Uint8 CommonXml::GetGamepadAxisFromInputXml(string xmlInput)
{
	if (gamepadStrings.count(xmlInput) == 0)
		return SDL_CONTROLLER_AXIS_INVALID;
	else return gamepadStrings.at(xmlInput);
}
