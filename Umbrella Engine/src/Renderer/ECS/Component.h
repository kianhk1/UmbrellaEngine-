#pragma once
#include "../API/GraphicsAPI/openglGraphicAPI.h"

enum class ProjectionType
{
	Perspective,
	Orthographic
};
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
	DATA::ShaderData shaderdata;
};
class TransformComponent : public Component {
public:
	TransformComponent(std::shared_ptr<API::GraphicsAPI> Graphic, DATA::ShaderData Shader) : Component(Graphic), shader(Shader) {}
	TransformComponent(std::shared_ptr<API::GraphicsAPI> Graphic) : Component(Graphic) {}
	// Transform
	DATA::ShaderData shader;
	glm::vec3 position = glm::vec3(0.0f);
	glm::quat rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	glm::mat4 modelMatrix = glm::mat4(1.0f);

	glm::vec3 forward = glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 right = glm::cross(forward, up);

	TransformComponent* parent = nullptr;
	void updateTransform() {
		glm::mat4 localMatrix =
			glm::translate(glm::mat4(1.0f), position) *
			glm::mat4_cast(rotation) *
			glm::scale(glm::mat4(1.0f), scale);
		if (parent) modelMatrix = parent->modelMatrix * localMatrix;
		else modelMatrix = localMatrix;

		//forward = glm::vec3(modelMatrix[2]); // محور Z جهانی شده
		//up = glm::vec3(modelMatrix[1]); // محور Y جهانی شده
		//right = glm::vec3(modelMatrix[0]);
		right = glm::normalize(glm::cross(forward, up));
		/*Logger::WARN("x:" + to_string(right.x) + "\n");
		Logger::WARN("y:" + to_string(right.y) + "\n");
		Logger::WARN("z:" + to_string(right.z) + "\n");*/

	}
	void Start() override {

	}
	void Update(float dt) override {
		if(shader.programID) {
			//Logger::WARN("iuvyuftjc");
			updateTransform();
			graphic->useShader(shader);
			graphic->set_mat4(shader.programID, modelMatrix, "model");
		}
	}
	void settarget(glm::vec3 Forward) {
		forward = Forward;
		right = glm::cross(forward, up);
	}
};
class MeshComponent : public Component {
public:
	MeshComponent(std::shared_ptr<API::GraphicsAPI> Graphic,
		std::vector<float>& vertices,
		std::vector<unsigned int>& indices,
		DATA::ShaderData Shader)
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
	std::string name = "";
	DATA::ShaderData shader;
	DATA::MeshData* meshdata;
};
class Texture2DComponent : public Component {
public:
	Texture2DComponent(
		std::shared_ptr<API::GraphicsAPI> Graphic,
		DATA::Texture2DData* T_data,
		DATA::ShaderData Shader)
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
	DATA::Texture2DData* t_data;
	DATA::ShaderData shader;
};
class CameraComponent : public Component {
public:
	CameraComponent(
		std::shared_ptr<API::GraphicsAPI> Graphic,
		ProjectionType Type,
		float AspectRatio,
		TransformComponent* Transform)
		: Component(Graphic), type(Type), transform(Transform) {
		cameradata.AspectRatio = AspectRatio;
	}
	void Update(float dt) override {
		//if (transform) Warn("jyvjgghjhvjgh\n");
		switch (type)
		{
		case ProjectionType::Perspective:
			cameradata.projection = glm::perspective(glm::radians(cameradata.fov), cameradata.AspectRatio, cameradata.NCP, cameradata.FCP);
			break;
		case ProjectionType::Orthographic:
			cameradata.projection = glm::ortho(0.0f, cameradata.windowWidth, 0.0f, cameradata.windowHeight, -1.0f, 1.0f);
			break;
		default:
			break;
		}
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		transform->forward = glm::normalize(direction);
		transform->right = glm::normalize(glm::cross(transform->forward, transform->up));
		cameradata.view = glm::lookAt(transform->position, transform->position + transform->forward, transform->up);
	}
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
		static float lastX = 400, lastY = 300;

		CameraComponent* cam = static_cast<CameraComponent*>(glfwGetWindowUserPointer(window));
		if (cam->firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			cam->firstMouse = false;
		}
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;
		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		yaw += xoffset;
		pitch += yoffset;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}
	TransformComponent* transform = nullptr;
	DATA::CameraData cameradata;
	ProjectionType type;
	int UBO_ID;
	float sensitivity = 0.1f;
	static float yaw, pitch;
	static bool firstMouse;
};
float CameraComponent::yaw, CameraComponent::pitch;
bool CameraComponent::firstMouse = true;
class aa : public Component {
public:
	aa(
		std::shared_ptr<API::GraphicsAPI> Graphic)
		: Component(Graphic) {}
};
class LightComponent : public Component {
public:
	LightComponent(
		std::shared_ptr<API::GraphicsAPI> Graphic,
		glm::vec3 lightcolor)
		: Component(Graphic) {
		light.lightcolor = lightcolor;
	}
	int UBO_ID;
	DATA::LightData light;
};
class MaterialComponent : public Component {
public:
	MaterialComponent(
		std::shared_ptr<API::GraphicsAPI> Graphic,
		const std::string& albedoTexturepath,
		const std::string& normalMappath = "",
		const std::string& specularMappath = "",
		const std::string& path_Vertex_Shader = "",
		const std::string& path_Fragment_Shader = "")
		: Component(Graphic) {
		material.albedoTexturedata.t_path = albedoTexturepath;
		material.normalMapdata.t_path = normalMappath;
		material.specularMapdata.t_path = specularMappath;

		material.albedoTexturedata.t_unit = GL_TEXTURE0;
		material.specularMapdata.t_unit = GL_TEXTURE1;
		material.normalMapdata.t_unit = GL_TEXTURE2;


		shader = new ShaderComponent(graphic, path_Vertex_Shader, path_Fragment_Shader);
		albedoTexture = new Texture2DComponent(graphic, &material.albedoTexturedata, shader->shaderdata);
		normalMap = new Texture2DComponent(graphic, &material.normalMapdata, shader->shaderdata);
		specularMap = new Texture2DComponent(graphic, &material.specularMapdata, shader->shaderdata);
	}
	// تنظیم رنگ پایه
	#define color(r,g,b,a) glm::vec4(r,g,b,a)
	void setBaseColor(float r, float g, float b, float a = 1.0f) {
		material.baseColor = color(r, g, b, a);
	}
	void setBaseColor(const glm::vec4& color) {
			material.baseColor = color;
	}

	// تنظیم مسیر تکسچر رنگ پایه
	void setAlbedoTexture(const std::string& path) {
		material.albedoTexturedata.t_path = path;
	}

	// تنظیم مسیر تکسچر معمولی
	void setNormalMapTexture(const std::string& path) {
		material.normalMapdata.t_path = path;
	}

	// تنظیم مسیر تکسچر بازتاب (Specular Map)
	void setSpecularMapTexture(const std::string& path) {
		material.specularMapdata.t_path = path;
	}

	// تنظیم رنگ بازتاب (Specular Color)
	void setSpecularColor(float r, float g, float b) {
		material.specularColor = color(r, g, b, 1.0f);
	}

	// تنظیم میزان درخشندگی (Shininess)
	void setShininess(float shininess_value) {
		material.shininess = shininess_value;
	}

	// تنظیم میزان شفافیت (Transparency/Opacity)
	void setTransparency(float alpha) {
		material.transparency = alpha;
		// اگر شفافیت کمتر از 1 باشه، ممکنه نیاز باشه flag ای برای فعال کردن blending تنظیم بشه.
	}

	// --- دریافت کننده‌ها (Getters) ---

	const glm::vec4& getBaseColor() const { return material.baseColor; }
	const std::string& getAlbedoTexturePath() const { return material.albedoTexturedata.t_path; }
	const std::string& getNormalMapTexturePath() const { return material.normalMapdata.t_path; }
	const std::string& getSpecularMapTexturePath() const { return material.specularMapdata.t_path; }
	const glm::vec3& getSpecularColor() const { return material.specularColor; }
	float getShininess() const { return material.shininess; }
	float getTransparency() const { return material.transparency; }

	// --- متدهای کمکی ---

	// بررسی اینکه آیا تکسچر رنگ پایه تنظیم شده است؟
	bool hasAlbedoTexture() const { return !material.albedoTexturedata.t_path.empty(); }
	bool hasNormalMapTexture() const { return !material.normalMapdata.t_path.empty(); }
	bool hasSpecularMapTexture() const { return !material.specularMapdata.t_path.empty(); }

	void Start() override {
		albedoTexture->set_texture("albedoTexture");
		normalMap->set_texture("normalTexture");
		specularMap->set_texture("specularTexture");

		//graphic->set_float(shader->shaderdata.programID, material.shininess, "");
		//graphic->set_vec4(shader->shaderdata.programID, material.baseColor, "");
		//graphic->set_vec3(shader->shaderdata.programID, material.specularColor, "");
	}
	void Update(float dt) override {
		albedoTexture->Bind();
		normalMap->Bind();
		specularMap->Bind();

		//graphic->set_float(shader->shaderdata.programID, material.shininess, "");
		//graphic->set_vec4(shader->shaderdata.programID, material.baseColor, "");
		//graphic->set_vec3(shader->shaderdata.programID, material.specularColor, "");
	}
	ShaderComponent* shader;
private:
	DATA::MaterialData material;

	Texture2DComponent* albedoTexture;
	Texture2DComponent* normalMap;
	Texture2DComponent* specularMap;
};