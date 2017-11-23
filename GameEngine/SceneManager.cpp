
#include "SceneManager.h"

SceneManager *SceneManager::m_instance = nullptr;

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	if (nullptr != m_instance) {
		delete m_instance;
	}
}

SceneManager * SceneManager::GetInstance()
{
	if (nullptr == m_instance) {
		m_instance = new SceneManager;
	}

	return m_instance;
}

bool SceneManager::Init(std::string filepath)
{
	FILE *f = fopen(filepath.c_str(), "r");

	if (NULL == f) {
		std::cerr << "Invallid path for scene manager xml file." << std::endl;
		return false;
	}

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *string = new char[fsize + 1];
	int sz = fread(string, fsize, 1, f);
	fclose(f);

	string[fsize] = '\0';

	rapidxml::xml_document<> doc;
	doc.parse<0>(string);

	rapidxml::xml_node<> *pRoot = doc.first_node();

	//background
	rapidxml::xml_node<> *pBkg = pRoot->first_node("backgroundColor");

	if (nullptr == pBkg) {
		m_background_color = glm::vec3(0.f, 0.f, 0.f);
	}
	else {
		m_background_color.x = std::stof(pBkg->first_node("r")->value());
		m_background_color.y = std::stof(pBkg->first_node("g")->value());
		m_background_color.z = std::stof(pBkg->first_node("b")->value());
	}

	//cameras
	rapidxml::xml_node<> *pCameras = pRoot->first_node("cameras");
	if (nullptr == pCameras) {
		std::cerr << "Cameras are missing" << std::endl;
		return false;
	}

	for (rapidxml::xml_node<> *pCamera = pCameras->first_node("camera"); pCamera; pCamera = pCamera->next_sibling()) {
		rapidxml::xml_attribute<> *pAttribute = pCamera->first_attribute("id");

		if (nullptr == pAttribute) {
			std::cerr << "Camera id missing" << std::endl;
			return false;
		}

		std::string id = std::string(pAttribute->value());

		float translateSpeed, rotationSpeed, fov, cnear, cfar;
		glm::vec3 pos, target, up;

		rapidxml::xml_node<> *pPosition = pCamera->first_node("position");

		if (nullptr == pPosition) {
			pos = glm::vec3(0.f, 0.f, 0.f);
		}
		else {
			pos.x = std::stof(pPosition->first_node("x")->value());
			pos.y = std::stof(pPosition->first_node("y")->value());
			pos.z = std::stof(pPosition->first_node("z")->value());
		}

		rapidxml::xml_node<> *pTarget = pCamera->first_node("target");

		if (nullptr == pTarget) {
			target = glm::vec3(0.f, 0.f, -1.f);
		}
		else {
			target.x = std::stof(pTarget->first_node("x")->value());
			target.y = std::stof(pTarget->first_node("y")->value());
			target.z = std::stof(pTarget->first_node("z")->value());
		}

		rapidxml::xml_node<> *pUp = pCamera->first_node("up");

		if (nullptr == pUp) {
			up = glm::vec3(0.f, 1.f, 0.f);
		}
		else {
			up.x = std::stof(pUp->first_node("x")->value());
			up.y = std::stof(pUp->first_node("y")->value());
			up.z = std::stof(pUp->first_node("z")->value());
		}

		rapidxml::xml_node<> *pTranslationSpeed = pCamera->first_node("translationSpeed");

		if (nullptr == pTranslationSpeed) {
			translateSpeed = 10;
		}
		else {
			translateSpeed = std::stoi(pTranslationSpeed->value());
		}

		rapidxml::xml_node<> *pRotationSpeed = pCamera->first_node("rotationSpeed");

		if (nullptr == pRotationSpeed) {
			rotationSpeed = 0.3f;
		}
		else {
			rotationSpeed = std::stof(pRotationSpeed->value());
		}

		rapidxml::xml_node<> *pFov = pCamera->first_node("fov");

		if (nullptr == pFov) {
			fov = 45;
		}
		else {
			fov = std::stoi(pFov->value());
		}

		rapidxml::xml_node<> *pNear = pCamera->first_node("near");

		if (nullptr == pNear) {
			cnear = 0.2f;
		}
		else {
			cnear = std::stof(pNear->value());
		}

		rapidxml::xml_node<> *pFar = pCamera->first_node("far");

		if (nullptr == pFar) {
			cfar = 10000.f;
		}
		else {
			cfar = std::stof(pFar->value());
		}

		m_cameras[id] = new Camera(pos, target, up, translateSpeed, rotationSpeed, cnear, cfar, fov);
	}

	rapidxml::xml_node<> *pActiveCamera = pRoot->first_node("activeCamera");
	if (nullptr == pActiveCamera) {
		m_active_camera = "1";
	}
	else {
		m_active_camera = pActiveCamera->value();
	}

	//objects
	rapidxml::xml_node<> *pObjects = pRoot->first_node("objects");
	if (nullptr == pObjects) {
		std::cerr << "Objects are missing" << std::endl;
		return false;
	}

	for (rapidxml::xml_node<> *pObject = pObjects->first_node("object"); pObject; pObject = pObject->next_sibling()) {
		rapidxml::xml_attribute<> *pAttribute = pObject->first_attribute("id");

		if (nullptr == pAttribute) {
			std::cerr << "Camera id missing" << std::endl;
			return false;
		}

		std::string id = std::string(pAttribute->value());

		rapidxml::xml_node<> *pModel = pObject->first_node("model");
		if (nullptr == pModel) {
			std::cerr << "Model id missing" << std::endl;
			return false;
		}

		std::string modelID = pModel->value();

		rapidxml::xml_node<> *pShader = pObject->first_node("shader");
		if (nullptr == pShader) {
			std::cerr << "Shader id missing" << std::endl;
			return false;
		}

		std::string shaderID = pShader->value();

		rapidxml::xml_node<> *pType = pObject->first_node("type");
		if (nullptr == pType) {
			std::cerr << "Object type missing" << std::endl;
			return false;
		}

		std::string type = pType->value();
		ObjectType ot = OT_NORMAL;

		if ("normal" == type) {
			ot = OT_NORMAL;
		}
		else if ("skybox" == type) {
			ot = OT_SKYBOX;
		}
		else if ("terrain" == type) {
			ot = OT_TERRAIN;
		}

		bool depthTest = true;
		rapidxml::xml_node<> *pDepthTest = pObject->first_node("depthTest");
		if (nullptr != pDepthTest) {
			depthTest = pDepthTest->value() == "true" ? true : false;
		}

		bool blend = true;
		rapidxml::xml_node<> *pBlend = pObject->first_node("blend");
		if (nullptr != pBlend) {
			blend = pBlend->value() == "true" ? true : false;
		}

		std::string name = "";
		rapidxml::xml_node<> *pName = pObject->first_node("name");
		if (nullptr != pName) {
			blend = pName->value();
		}

		rapidxml::xml_node<> *pTextures = pObject->first_node("textures");
		std::vector<std::string> texture_ids;
		if (nullptr != pTextures) {
			for (rapidxml::xml_node<> *pTexture = pTextures->first_node("texture"); pTexture; pTexture = pTexture->next_sibling()) {
				rapidxml::xml_attribute<> *pAttribute = pTexture->first_attribute("id");

				if (nullptr == pAttribute) {
					std::cerr << "Texture id missing" << std::endl;
					return false;
				}

				texture_ids.push_back(pAttribute->value());
			}
		}

		

		glm::vec3 pos;
		rapidxml::xml_node<> *pPosition = pObject->first_node("position");

		if (nullptr == pPosition) {
			pos = glm::vec3(0.f, 0.f, 0.f);
		}
		else {
			pos.x = std::stof(pPosition->first_node("x")->value());
			pos.y = std::stof(pPosition->first_node("y")->value());
			pos.z = std::stof(pPosition->first_node("z")->value());
		}

		glm::vec3 rot;
		rapidxml::xml_node<> *pRotation = pObject->first_node("rotation");

		if (nullptr == pRotation) {
			pos = glm::vec3(0.f, 0.f, 0.f);
		}
		else {
			rot.x = std::stof(pRotation->first_node("x")->value());
			rot.y = std::stof(pRotation->first_node("y")->value());
			rot.z = std::stof(pRotation->first_node("z")->value());
		}

		glm::vec3 scale;
		rapidxml::xml_node<> *pScale = pObject->first_node("scale");

		if (nullptr == pScale) {
			pos = glm::vec3(0.f, 0.f, 0.f);
		}
		else {
			scale.x = std::stof(pScale->first_node("x")->value());
			scale.y = std::stof(pScale->first_node("y")->value());
			scale.z = std::stof(pScale->first_node("z")->value());
		}

		glm::vec3 color;
		rapidxml::xml_node<> *pColor = pObject->first_node("color");

		if (nullptr == pColor) {
			color = glm::vec3(0.f, 0.f, 0.f);
		}
		else {
			color.x = std::stof(pColor->first_node("r")->value());
			color.y = std::stof(pColor->first_node("g")->value());
			color.z = std::stof(pColor->first_node("b")->value());
		}

		float selfRotateSpeed;
		rapidxml::xml_node<> *pSelfRotateSpeed = pObject->first_node("selfRotateSpeed");

		if (nullptr == pSelfRotateSpeed) {
			selfRotateSpeed = 0.1f;
		}
		else {
			selfRotateSpeed = std::stof(pSelfRotateSpeed->value());
		}

		rapidxml::xml_node<> *pWired = pObject->first_node("wired");
		bool wired = pWired ? true : false;

		// TODO: TRAJECTORY
		// TODO: LIGHTS

		SceneObject *object = nullptr;

		if (OT_NORMAL == ot) {
			object = new SceneObject(pos, rot, scale, depthTest, name);
			object->SetModel(ResourceManager::GetInstance()->LoadModel(modelID));
			object->SetShader(ResourceManager::GetInstance()->LoadShader(shaderID));
			for (std::string texID : texture_ids) {
				object->AddTexture(ResourceManager::GetInstance()->LoadTexture(texID));
			}

			m_objects[id] = object;
		}
	}
	//ambiental light
	//lights
	//debug settings

	delete string;

	glClearColor(m_background_color.x, m_background_color.y, m_background_color.z, 0.f);
	glEnable(GL_DEPTH_TEST);

	return true;
}

void SceneManager::Update()
{
}

void SceneManager::Draw()
{
	for (auto & obj : m_objects) {
		obj.second->Draw();
	}
}
