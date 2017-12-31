#include "SceneManager.h"
#include "ShaderProgram.h"
#include "GameObject.h"

SceneManager::SceneManager(ResourceManager* rM)
{
	sceneMgrConfigPath = GAMEDATA_PATH + "sceneMgrConfig.scn";
	resourceManager = rM;
}

SceneManager::~SceneManager()
{
}


XMLError SceneManager::LoadSceneDirectories()
{
	XMLDocument *gamedataXML = new XMLDocument();
	CommonXml::LoadXML(sceneMgrConfigPath, gamedataXML);

	XMLNode *SMCroot = gamedataXML->FirstChild();
	if (!SMCroot)
		return XML_ERROR_FILE_READ_ERROR;

	XMLElement *SMCdirectories = SMCroot->FirstChildElement("SceneDirectories");
	if (!SMCdirectories)
		return XML_ERROR_PARSING_ELEMENT;
	
	XMLElement *DirectoryItem = SMCdirectories->FirstChildElement("Scene");
	while (DirectoryItem)
	{
		int tOrder;
		string tDirectory;
		eResult = DirectoryItem->QueryIntAttribute("order", &tOrder);
		CommonXml::XmlErrorCheck(eResult, "SceneDirectoryItem loading");
		tDirectory = DirectoryItem->GetText();
		scenes[tOrder] = tDirectory;
		DirectoryItem = DirectoryItem->NextSiblingElement("Scene");
	}
	delete gamedataXML;
	return XML_SUCCESS;
}

XMLError SceneManager::LoadScene(int sceneOrder, Scene* currentScene)
{
	XMLDocument *gamedataXML = new XMLDocument();
	CommonXml::LoadXML(GAMEDATA_PATH + scenes[sceneOrder], gamedataXML);
	XMLNode *sceneRoot = gamedataXML->FirstChild();
	if (!sceneRoot)
		return XML_ERROR_FILE_READ_ERROR;

	XMLElement *sceneData = sceneRoot->FirstChildElement("ModelList");
	if (!sceneData)
		return XML_ERROR_PARSING_ELEMENT;

	//a quick routine to count all the child elements
	int count = 0;
	for (XMLElement *elem = sceneData->FirstChildElement("Model");
		elem; elem = elem->NextSiblingElement("Model"))
		count++;

	int curr = 1;
	XMLElement *sceneItem = sceneData->FirstChildElement("Model");
	while (sceneItem) 
	{
		string name = sceneItem->GetText();
		printf("Loading model %d/%d - %s\n", curr++, count, name.c_str());
		resourceManager->AddModel(name);
		sceneItem = sceneItem->NextSiblingElement("Model");
	}

	
	sceneData = sceneRoot->FirstChildElement("TextureList");
	if (!sceneData)
		return XML_ERROR_PARSING_ELEMENT;
	
	count = 0;
	for (XMLElement *elem = sceneData->FirstChildElement("Texture");
	elem; elem = elem->NextSiblingElement("Texture"))
		count++;

	curr = 1;
	sceneItem = sceneData->FirstChildElement("Texture");
	while (sceneItem) 
	{
		string name = sceneItem->GetText();
		printf("Loading texture %d/%d - %s\n", curr++, count, name.c_str());
		resourceManager->AddTexture(name);
		sceneItem = sceneItem->NextSiblingElement("Texture");
	}

	sceneData = sceneRoot->FirstChildElement("FontList");
	if (!sceneData)
		return XML_ERROR_PARSING_ELEMENT;

	count = 0;
	for (XMLElement *elem = sceneData->FirstChildElement("Font");
	elem; elem = elem->NextSiblingElement("Font"))
		count++;

	curr = 1;
	sceneItem = sceneData->FirstChildElement("Font");
	while (sceneItem) 
	{
		string name = sceneItem->GetText();
		printf("Loading font %d/%d - %s\n", curr++, count, name.c_str());
		resourceManager->AddFont(name);
		sceneItem = sceneItem->NextSiblingElement("Font");
	}

	sceneData = sceneRoot->FirstChildElement("ShaderList");
	if (!sceneData)
		return XML_ERROR_PARSING_ELEMENT;

	count = 0;
	for (XMLElement *elem = sceneData->FirstChildElement("Shader");
	elem; elem = elem->NextSiblingElement("Shader"))
		count++;

	curr = 1;
	sceneItem = sceneData->FirstChildElement("Shader");
	while (sceneItem)
	{
		string name = sceneItem->GetText();
		printf("Loading shader %d/%d - %s\n", curr++, count, name.c_str());
		string vert = sceneItem->Attribute("vert");
		string frag = sceneItem->Attribute("frag");
		ShaderProgram *shader = new ShaderProgram(SHADER_PATH + vert, SHADER_PATH + frag);
		shader->Link();
		resourceManager->AddShader(shader, name);
		sceneItem = sceneItem->NextSiblingElement("Shader");
	}

	sceneData = sceneRoot->FirstChildElement("GameObjectList");
	if (!sceneData)
		return XML_ERROR_PARSING_ELEMENT;

	count = 0;
	for (XMLElement *elem = sceneData->FirstChildElement("GameObject");
	elem; elem = elem->NextSiblingElement("GameObject"))
		count++;

	curr = 1;
	sceneItem = sceneData->FirstChildElement("GameObject");
	while (sceneItem) 
	{
		string tName = sceneItem->Attribute("name");
		printf("Instantiating gameobject %d/%d - %s\n", curr++, count, tName.c_str());
		bool hasRenderer = true;
		sceneItem->QueryBoolAttribute("hasRenderer", &hasRenderer);
		string textures = sceneItem->Attribute("texture");
		vector<string> splits = split(textures, ',');

		string tModelName = sceneItem->Attribute("model");
		string tShaderProgramName = sceneItem->Attribute("shader");

		vec3 tPos;
		sceneItem->QueryFloatAttribute("posx", &tPos.x);
		sceneItem->QueryFloatAttribute("posy", &tPos.y);
		sceneItem->QueryFloatAttribute("posz", &tPos.z);
		vec3 tRot;
		sceneItem->QueryFloatAttribute("rotx", &tRot.x);
		sceneItem->QueryFloatAttribute("roty", &tRot.y);
		sceneItem->QueryFloatAttribute("rotz", &tRot.z);
		vec3 tScale;
		sceneItem->QueryFloatAttribute("scalex", &tScale.x);
		sceneItem->QueryFloatAttribute("scaley", &tScale.y);
		sceneItem->QueryFloatAttribute("scalez", &tScale.z);

		GameObject *go;
		if (hasRenderer) 
		{
			Renderer *renderer = new Renderer();
			renderer->SetTransparent(sceneItem->Attribute("transparent", "true"));
			if (tModelName.length() > 0)
			{
				Model *m = resourceManager->GetModel(tModelName);
				if (m)
					renderer->SetModel(m, GL_TRIANGLES);
			}

			for (int i = 0; i < splits.size(); i++)
				renderer->AddTexture(resourceManager->GetTexture(splits[i]));

			if (tShaderProgramName.length() > 0)
			{
				ShaderProgram *shader = resourceManager->GetShader(tShaderProgramName);
				if (shader)
					renderer->SetShaderProgram(shader);
			}
			go = currentScene->AddGameObject(tName, tPos, tRot, tScale, renderer);
		}
		else 
			go = currentScene->AddGameObject(tName, tPos, tRot, tScale);

		XMLElement *gameObjectElement = sceneItem->FirstChildElement("Material");
		if (gameObjectElement) 
		{
			go->GetMaterial()->diffuseColor = CommonXml::GetVec4FromString(gameObjectElement->Attribute("diffuse"));
			go->GetMaterial()->specPower = stof(gameObjectElement->Attribute("specularPower"));
			go->GetMaterial()->specIntensity = stof(gameObjectElement->Attribute("specularIntensity"));
		}

		gameObjectElement = sceneItem->FirstChildElement("ComponentList");
		if (!gameObjectElement)
			return XML_ERROR_PARSING_ELEMENT;

		XMLElement *componentElement = gameObjectElement->FirstChildElement("Component");
		while (componentElement) 
		{
			string compID = componentElement->Attribute("ID");
			XMLElement * attributesElement = componentElement->FirstChildElement("Attributes");
			currentScene->AttachComponent(compID, go, attributesElement);
			componentElement = componentElement->NextSiblingElement("Component");
		}
		sceneItem = sceneItem->NextSiblingElement("GameObject");
	}

	if (!currentScene->GetMainDirLight())
	{
		GameObject* dLight = new GameObject();
		dLight->AttachComponent(new Light(vec4{ 1,1,1,1 }, E_LightState::DIRECTIONAL));
		currentScene->SetMainDirLight(dLight);
	}
	delete gamedataXML;
	return XML_SUCCESS;
}

XMLError SceneManager::LoadScene(string lvlName, Scene* currentScene)
{
	int sceneOrder = -1;
	for (auto iter = scenes.begin(); iter != scenes.end(); iter++)
	{
		if (iter->second == lvlName + ".scn")
			sceneOrder = iter->first;
	}
	if (sceneOrder == -1) //failed to find a proper scene
		return XML_ERROR_FILE_NOT_FOUND;
	
	return LoadScene(sceneOrder, currentScene);
}
