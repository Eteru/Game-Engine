
#include "SceneManager.h"

SceneManager *SceneManager::m_instance = nullptr;

SceneManager::SceneManager()
{
	m_octree = new Octree(glm::vec3(0), glm::vec3(50));
}

SceneManager::~SceneManager()
{
	if (nullptr != m_instance) {
		delete m_instance;
	}

	delete m_octree;
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

		float speed, sensitivity, fov, cnear, cfar;
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

		rapidxml::xml_node<> *pSpeed = pCamera->first_node("speed");

		if (nullptr == pSpeed) {
			speed = 0.005;
		}
		else {
			speed = std::stof(pSpeed->value());
		}

		rapidxml::xml_node<> *pSensitivity = pCamera->first_node("sensitivity");

		if (nullptr == pSensitivity) {
			sensitivity = 0.3f;
		}
		else {
			sensitivity = std::stof(pSensitivity->value());
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

		m_cameras[id] = std::shared_ptr<Camera>(new Camera(pos, target, up, fov, 1.7777f, cnear, cfar, speed, sensitivity));
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
			std::cerr << "Object id missing" << std::endl;
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

		// TODO: TRAJECTORY
		// TODO: LIGHTS

		SceneObject *object = nullptr;

		object = new SceneObject(pos, rot, scale);
		object->SetModel(ResourceManager::GetInstance()->LoadModel(modelID));
		object->SetShader(ResourceManager::GetInstance()->LoadShader(shaderID));
		for (std::string texID : texture_ids) {
			object->AddTexture(ResourceManager::GetInstance()->LoadTexture(texID));
		}

		m_objects[id] = std::shared_ptr<SceneObject>(object);
		m_octree->Insert(object);
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
	Camera *cam = GetActiveCamera();
	m_octree->Draw(cam->GetViewMatrix(), cam->GetProjectionMatrix());
	m_octree->DrawContainedObjetcs(cam->GetFrustrum());
}
