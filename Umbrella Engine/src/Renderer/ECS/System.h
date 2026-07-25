#pragma once
#include "../API/GraphicsAPI/GraphicsAPI.h"
#include "../../Input & Output Manager/Input System/InputSystem.h"
#include "../Loader/AssetManager.h"
#include "Entity.h"

class SystemManager {
public:

};

class System {
public:
	~System() = default;
	virtual void Start() = 0;
	virtual void Update(float dt) = 0;
};

class RenderSystem : public System {
public:
	void Start() override {
		auto view = registry.view<TransformComponent, MeshRendererComponent>();
		view.each(
			[this](auto entity,
				TransformComponent& transform,
				MeshRendererComponent& render){
					// init mesh
					auto modeldata = Engine::AssetManager::GetInstance().GetModel(render.modelhandle);
					auto shaderldata = Engine::AssetManager::GetInstance().GetShader(render.shader);

					for (auto& part : modeldata->parts) {
						 
						part.mesh = Engine::API::createMesh(part.mesh.vertices, part.mesh.indices); 
						Engine::API::setAttrib(part.mesh, 0, 3, 14, 0);
						Engine::API::setAttrib(part.mesh, 1, 3, 14, 3);
						Engine::API::setAttrib(part.mesh, 2, 2, 14, 6);
						Engine::API::setAttrib(part.mesh, 3, 3, 14, 8);
						Engine::API::setAttrib(part.mesh, 4, 3, 14, 11);
						for (auto& it : part.material.textures) {
							Engine::API::SetUniform(shaderldata->programID,
								Engine::AssetManager::GetInstance().GetTexture(it.second.ID)->unit, "albedoTexture");
							//Warn(Engine::CORE::LogCategory::API, "sfdsfdsfdsfdf");
						}
					} 
					modeldata->root->each([&](Engine::DATA::Node& node, int depth) {
						Warn(Engine::CORE::LogCategory::API, depth ? '|' : '\0', std::string(depth, '-'), depth ? '>' : '\0', node.name);
						});
		 
			});
	}
	void Update(float dt) override {
		auto view = registry.view<TransformComponent, MeshRendererComponent>();
		

		view.each( 
			[this](auto entity,
				TransformComponent& transform, 
				MeshRendererComponent& render) { 
					auto modeldata = Engine::AssetManager::GetInstance().GetModel(render.modelhandle);
					auto shaderldata = Engine::AssetManager::GetInstance().GetShader(render.shader);

					Engine::API::useShader(*shaderldata);
					modeldata->root->each([&](Engine::DATA::Node& node, int depth) {
						Engine::API::SetUniform(shaderldata->programID, node.localTransform * transform.modelMatrix, "model"); 
						
						
						for(const int& i : node.meshIndices)
						{
							for (const auto& it : modeldata->parts[i].material.textures)
							{
								Engine::API::Bind(Engine::AssetManager::GetInstance().GetTexture(it.second));
							}
							for (const auto& it : modeldata->parts[i].material.uniforms)
							{ 
								
								Engine::API::SetUniform(shaderldata->programID, it.second, it.first.c_str());
							}
							Engine::API::drawMesh(modeldata->parts[i].mesh, render.state);
						}
						});
					updateTransform(transform);
					
			});
	}
private:
	void updateTransform(TransformComponent& transform) {
		glm::mat4 localMatrix =
			glm::translate(glm::mat4(1.0f), transform.position) *
			glm::rotate(glm::mat4(1.0f), transform.rotation.x, glm::vec3(1, 0, 0)) * // Pitch
			glm::rotate(glm::mat4(1.0f), transform.rotation.y, glm::vec3(0, 1, 0)) * // Yaw
			glm::rotate(glm::mat4(1.0f), transform.rotation.z, glm::vec3(0, 0, 1)) * // Roll
			glm::scale(glm::mat4(1.0f), transform.scale);
		if (transform.parent) transform.modelMatrix = transform.parent->modelMatrix * localMatrix;
		else transform.modelMatrix = localMatrix;

		//forward = glm::vec3(modelMatrix[2]); // محور Z جهانی شده
		//up = glm::vec3(modelMatrix[1]); // محور Y جهانی شده
		//right = glm::vec3(modelMatrix[0]);
		transform.right = glm::normalize(glm::cross(transform.forward, transform.up));
		/*Logger::WARN("x:" + to_string(right.x) + "\n");
		Logger::WARN("y:" + to_string(right.y) + "\n");
		Logger::WARN("z:" + to_string(right.z) + "\n");*/

	}
};

class CameraSystem : public System {
public:
	CameraSystem(std::shared_ptr<Engine::DATA::windowData> window) : Window(window) {}
	void Start() override {
		auto view = registry.view<TransformComponent, CameraComponent>();
		
		view.each(
			[this](auto entity,
				TransformComponent& transform,
				CameraComponent& cam) {
					cam.UBO_ID = Engine::API::createUBO(144);//projection + view + position
					Engine::API::BindBuffer(cam.UBO_ID, 0, 0, 144);
					cam.cameradata.windowHeight = Window->display.height;
					cam.cameradata.windowWidth = Window->display.width;
					cam.cameradata.AspectRatio = cam.cameradata.windowWidth / cam.cameradata.windowHeight;
					switch (cam.type)
					{
					case ProjectionType::Perspective:
						cam.cameradata.projection = glm::perspective(glm::radians(cam.cameradata.fov), 
																	cam.cameradata.AspectRatio, cam.cameradata.NCP, cam.cameradata.FCP);
						break;
					case ProjectionType::Orthographic:
						cam.cameradata.projection = glm::ortho(0.0f, cam.cameradata.windowWidth, 
																0.0f, cam.cameradata.windowHeight, -1.0f, 1.0f);
						break;
					default:
						break;
					}
			});
	}
	void Update(float dt) override {
		auto view = registry.view<TransformComponent, CameraComponent>();
		
		view.each(
			[this](auto entity,
				TransformComponent& transform,
				CameraComponent& cam) {
					if (cam.ismoving) {
						Engine::Event::EventManager::GetInstance().Subscribe("windowresize", [&](void* d) {
							auto* size = static_cast<Engine::DATA::Size*>(d);
							cam.cameradata.AspectRatio = static_cast<float>(size->width) / size->height;
							std::cout << "aa:" << cam.cameradata.AspectRatio << '\n';
							switch (cam.type)
							{
							case ProjectionType::Perspective:
								cam.cameradata.projection = glm::perspective(glm::radians(cam.cameradata.fov), 
																			cam.cameradata.AspectRatio, cam.cameradata.NCP, cam.cameradata.FCP);
								break;
							case ProjectionType::Orthographic:
								cam.cameradata.projection = glm::ortho(0.0f, static_cast<float>(size->width), 
																		0.0f, static_cast<float>(size->height), -1.0f, 1.0f);
								break;
							default:
								break;
							}
							});
						

						UpdateCameraRotation(cam, Window);

						glm::vec3 direction;
						direction.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
						direction.y = sin(glm::radians(cam.pitch));
						direction.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));

						transform.forward = glm::normalize(direction);
						transform.right = glm::normalize(glm::cross(transform.forward, transform.up)); 

						cam.cameradata.view = glm::lookAt(transform.position, transform.position + transform.forward, transform.up);

						Engine::API::UpdateBuffer(cam.UBO_ID, glm::value_ptr(cam.cameradata.projection), 0, sizeof(glm::mat4));
						Engine::API::UpdateBuffer(cam.UBO_ID, glm::value_ptr(cam.cameradata.view), sizeof(glm::mat4), sizeof(glm::mat4));
						Engine::API::UpdateBuffer(cam.UBO_ID, glm::value_ptr(transform.position), 2 * sizeof(glm::mat4), sizeof(glm::vec4));

						Move(transform, 0.1);
					}
			});
	}
private:
	std::shared_ptr<Engine::DATA::windowData> Window;
	void UpdateCameraRotation(CameraComponent& cam, std::shared_ptr<Engine::DATA::windowData> window) {

		static float lastX = window->display.width/2, lastY = window->display.height / 2;

		if (cam.firstMouse)
		{
			lastX = window->mousepos.x;
			lastY = window->mousepos.y;
			cam.firstMouse = false;
		}
		float xoffset = window->mousepos.x - lastX;
		float yoffset = lastY - window->mousepos.y;
		lastX = window->mousepos.x;
		lastY = window->mousepos.y;
		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		cam.yaw += xoffset;
		cam.pitch += yoffset;
		if (cam.pitch > 89.0f)
			cam.pitch = 89.0f;
		if (cam.pitch < -89.0f)
			cam.pitch = -89.0f;
	}
	void Move(TransformComponent& transform, float dt) {

		float velocity = 2 * dt;
		if (Engine::API::Input::IsKeyPressed(Window,Engine::API::KeyboardKey::KEY_W)) {
			transform.position += transform.forward * velocity;// جلو
		}
		if (Engine::API::Input::IsKeyPressed(Window, Engine::API::KeyboardKey::KEY_S)) {
			transform.position -= transform.forward * velocity;// عقب 
		}
		if (Engine::API::Input::IsKeyPressed(Window, Engine::API::KeyboardKey::KEY_A)) {
			transform.position -= transform.right * velocity; 
			//transform->position.x -= transform->forward.x * velocity;// چپ               
		}
		if (Engine::API::Input::IsKeyPressed(Window, Engine::API::KeyboardKey::KEY_D)) {
			transform.position += transform.right * velocity;
			//transform->position.x += transform->forward.x * velocity;// راست               
		}
		if (Engine::API::Input::IsKeyPressed(Window, Engine::API::KeyboardKey::KEY_SPACE)) {
			transform.position += transform.up * velocity;// بالا 
		}
		if (Engine::API::Input::IsKeyPressed(Window, Engine::API::KeyboardKey::KEY_LEFT_SHIFT)) {
			transform.position -= transform.up * velocity;// پایین 
		}
	}
};

