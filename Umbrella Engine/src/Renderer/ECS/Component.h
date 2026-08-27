#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../Core/Data/Data.h"

enum ProjectionType
{
	Perspective,
	Orthographic
};
class TransformComponent {
public:

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	glm::mat4 modelMatrix = glm::mat4(1.0f);

	glm::vec3 forward = glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 right = glm::cross(forward, up);

	TransformComponent* parent = nullptr;
};
class MeshRendererComponent {
public:
	Engine::DATA::ModelHandle modelhandle;
	Engine::DATA::ShaderHandle shader;
	Engine::DATA::RenderState state;

};
class CameraComponent {
public:
	Engine::DATA::CameraData cameradata;
	int type = Perspective;
	int UBO_ID;
	float sensitivity = 0.1f;
	float yaw, pitch;
	bool firstMouse;
	bool ismoving = true;
	bool isactive = true;
	bool dirty = false;
};
enum LightType {
	Directional = 0, 
	Point, 
	Spot 
};
class LightComponent {
public:
	int UBO_ID;
	Engine::DATA::LightData light;
	unsigned int dephtmao, depthFBO;
	glm::mat4 lightSpaceMatrix;
	bool shadow = true;
	int type = Point;
};
class ScriptComponent {
public:
	int id;
	std::string name;
};