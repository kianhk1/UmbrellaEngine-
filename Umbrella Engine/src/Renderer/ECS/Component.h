#pragma once
#include "../API/GraphicsAPI/openglGraphicAPI.h"
class Component {
public:
	Component(std::shared_ptr<API::GraphicsAPI> Graphic) {
		graphic = Graphic;
	}
	bool isActive = true;
	virtual ~Component() = default;
	virtual void Start() {}
	virtual void Update(float dt) {}
	std::shared_ptr<API::GraphicsAPI> graphic;
};

class ShaderComponent : public Component {
public:
	ShaderComponent(
		std::shared_ptr<API::GraphicsAPI> Graphic,
		const std::string& path_Vertex_Shader, 
		const std::string& path_Fragment_Shader) 
		: Component(Graphic){
		shaderdata = graphic->createShader(path_Vertex_Shader, path_Fragment_Shader);
	}
	void use() {
		graphic->useShader(shaderdata);
	}
	ShaderData shaderdata;
};
class TransformComponent : public Component {
public:
	TransformComponent(std::shared_ptr<API::GraphicsAPI> Graphic, ShaderData Shader) : Component(Graphic), shader(Shader) {}
	// Transform
	ShaderData shader;
	glm::vec3 position = glm::vec3(0.0f);
	glm::quat rotation;
	glm::vec3 scale = glm::vec3(1.0f);
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	TransformComponent* parent = nullptr;
	void updateTransform() {
		glm::mat4 localMatrix =
			glm::translate(glm::mat4(1.0f), position) *
			glm::mat4_cast(rotation) *
			glm::scale(glm::mat4(1.0f), scale);
		if (parent) modelMatrix = parent->modelMatrix * localMatrix;
		else modelMatrix = localMatrix;
	}
	void Start() override {

	}
	void Update(float dt) override {
		updateTransform();
		graphic->useShader(shader);
		graphic->set_mat4(shader.programID, modelMatrix, "model");
	}
};
class MeshComponent : public Component {
public:
	MeshComponent(std::shared_ptr<API::GraphicsAPI> Graphic,
		std::vector<float>& vertices,
		std::vector<unsigned int>& indices,
		ShaderData Shader)
		: Component(Graphic), shader(Shader) {
		meshdata = graphic->createMesh(vertices, indices);
	}
	void setAttrib(int a, int b, int c, int d) {
		graphic->setAttrib(meshdata, a, b, c, d);
	}
	void Start() override {

	}
	void Update(float dt) override {

	}
	void draw() {
		graphic->useShader(shader);
		graphic->drawMesh(meshdata);
	}
	ShaderData shader;
	MeshData* meshdata;
};
class Texture2DComponent : public Component {
public:
	Texture2DComponent(std::shared_ptr<API::GraphicsAPI> Graphic,
		Texture2DData* T_data,
		ShaderData Shader)
		: Component(Graphic), t_data(T_data), shader(Shader) {
		graphic->Load2DTexture(t_data);
	}
	void set_texture(const char* name) {
		graphic->useShader(shader);
		graphic->set_int(shader.programID, t_data->t_unit - GL_TEXTURE0, name);
	}
	void Bind() {
		graphic->Bind(t_data);
	}
	void Start() override {

	}
	void Update(float dt) override {

	}
	Texture2DData* t_data;
	ShaderData shader;
};
