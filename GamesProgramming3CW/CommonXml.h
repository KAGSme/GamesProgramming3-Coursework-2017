#pragma once
#include "common.h"
#include "tinyxml2.h"

using namespace tinyxml2;

class CommonXml 
{
public:
	static void XmlErrorCheck(XMLError error, string msg);
	static void LoadXML(string filePath, XMLDocument* xml);
	static int GetSpecialKeyFromInputXml(string xmlInput);
	static Uint8 GetGamepadButtonFromInputXml(string xmlInput);
	static Uint8 GetGamepadAxisFromInputXml(string xmlInput);
	static vec4 GetVec4FromString(string xmlInput);
};