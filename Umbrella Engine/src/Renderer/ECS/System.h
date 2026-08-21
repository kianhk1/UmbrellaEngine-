#pragma once
#include "../API/GraphicsAPI/GraphicsAPI.h"
#include "../../Input & Output Manager/Input System/InputSystem.h"
#include "../Loader/AssetManager.h"
#include "Entity.h"
#include "../../Scene/Scene.h"

class ObjCube
{
public:
	ObjCube(std::shared_ptr<Engine::Scene::Scene> scene, std::string path) {
		auto& registry = scene->Registry();
		entity = registry.create();
		TransformComponent t;
		MeshRendererComponent m;
		m.modelhandle =
			Engine::AssetManager::GetInstance().LoadModel(path);
		t.scale = glm::vec3(1.0);
		m.shader =
			Engine::AssetManager::GetInstance().LoadShader("Shader/vertex_shader.glsl", "Shader/fragment_shader.glsl");
		registry.emplace<TransformComponent>(entity, t);
		registry.emplace<MeshRendererComponent>(entity, m);
	}
	ObjCube(std::shared_ptr<Engine::Scene::Scene> scene, int x, int y, int z) {
		auto& registry = scene->Registry();
		entity = registry.create();
		TransformComponent t;
		t.position = glm::vec3(x, y, z);
		LightComponent l;
		l.light.lightcolor = glm::vec3(1.0);
		l.type = Directional;
		MeshRendererComponent m;
		m.modelhandle =
			Engine::AssetManager::GetInstance().LoadModel("Assets/cube.gltf");
		t.scale = glm::vec3(3.0);
		m.shader =
			Engine::AssetManager::GetInstance().LoadShader("Shader/vertex_shader.glsl", "Shader/fragment_shader.glsl");
		registry.emplace<TransformComponent>(entity, t);
		registry.emplace<LightComponent>(entity, l);
		//registry.emplace<MeshRendererComponent>(Entity, m); 
	}
	ObjCube(std::shared_ptr<Engine::Scene::Scene> scene, std::string path, int a) {
		auto& registry = scene->Registry();
		entity = registry.create();
		TransformComponent t;
		MeshRendererComponent m;
		m.modelhandle =
			Engine::AssetManager::GetInstance().LoadModel("Assets/cube.gltf");
		t.scale = glm::vec3(3.0);
		m.shader =
			Engine::AssetManager::GetInstance().LoadShader("Shader/skybox_shader/vertex_skybox_shader.glsl", "Shader/skybox_shader/fragment_skybox_shader.glsl");
		//Engine::AssetManager::GetInstance().GetModel(m.modelhandle)->GetMaterial(0).material.uniforms.clear();
		Engine::AssetManager::GetInstance().GetModel(m.modelhandle)->GetMaterial(0).material.textures.emplace("skybox", Engine::AssetManager::GetInstance().LoadCubeMap(path));
		//m.state.depthtest = false;
		m.state.shadow = false;
		m.state.depthfunc = GL_LEQUAL;
		registry.emplace<TransformComponent>(entity, t);
		registry.emplace<MeshRendererComponent>(entity, m);
	}
	Entity entity;
private:

};

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
	RenderSystem(std::shared_ptr<Engine::Scene::Scene> scene) : ActiveScene(scene) {}
	void Start() override {
		auto& registry = ActiveScene->Registry();
		auto view = registry.view<TransformComponent, MeshRendererComponent>();
		colormap = Engine::API::createcolorhmap();
		colorFBO = Engine::API::createFBO(colormap, GL_COLOR_ATTACHMENT0);
		view.each(
			[this](auto entity,
				TransformComponent& transform,
				MeshRendererComponent& render){
					
					// init mesh
					auto modeldata = Engine::AssetManager::GetInstance().GetModel(render.modelhandle);
					auto shaderldata = Engine::AssetManager::GetInstance().GetShader(render.shader);
					if(modeldata) {
						for (auto& part : modeldata->parts) {

							part.mesh = Engine::API::createMesh(part.mesh.vertices, part.mesh.indices);
							Engine::API::setAttrib(part.mesh, 0, 3, 14, 0);
							Engine::API::setAttrib(part.mesh, 1, 3, 14, 3);
							Engine::API::setAttrib(part.mesh, 2, 2, 14, 6);
							Engine::API::setAttrib(part.mesh, 3, 3, 14, 8);
							Engine::API::setAttrib(part.mesh, 4, 3, 14, 11);
							
							for (auto& it : part.material.textures) {
								auto tex = Engine::AssetManager::GetInstance().GetTexture(it.second.ID);

								Engine::API::SetUniform(shaderldata->programID,
									Engine::AssetManager::GetInstance().GetTexture(it.second.ID)->unit, it.first.c_str());
								//Warn(Engine::CORE::LogCategory::API, "sfdsfdsfdsfdf");
							}
						}
						modeldata->root->each([&](Engine::DATA::Node& node, int depth) {
							Warn(Engine::CORE::LogCategory::API, depth ? '|' : '\0', std::string(depth, '-'), depth ? '>' : '\0', node.name);
							});
					}
		 
			});
	}
	void Update(float dt) override {
		auto& registry = ActiveScene->Registry();
		auto view = registry.view<TransformComponent, MeshRendererComponent>();
		Engine::API::clearBuffers(Engine::API::Buffers::COLOR);
		///Engine::API::BindFBO(colorFBO);
		view.each( 
			[this](auto entity,
				TransformComponent& transform, 
				MeshRendererComponent& render) { 
					auto modeldata = Engine::AssetManager::GetInstance().GetModel(render.modelhandle);
					auto shaderldata = Engine::AssetManager::GetInstance().GetShader(render.shader);
					
					Engine::API::useShader(*shaderldata);
					
					updateTransform(transform); 
					if (modeldata)
					{
						modeldata->root->each([&](Engine::DATA::Node& node, int depth) {
							Engine::API::SetUniform(shaderldata->programID, transform.modelMatrix * node.localTransform, "model");


							for (const int& i : node.meshIndices)
							{
								for (const auto& it : modeldata->parts[i].material.textures)
								{
									Engine::API::Bind(Engine::AssetManager::GetInstance().GetTexture(it.second));
									Engine::API::SetUniform(shaderldata->programID, Engine::AssetManager::GetInstance().GetTexture(it.second.ID)->unit, it.first.c_str());
								}
								for (const auto& it : modeldata->parts[i].material.uniforms)
								{
									Engine::API::SetUniform(shaderldata->programID, it.second, it.first.c_str());
								}
								if(render.state.shadow)
									Engine::API::SetUniform(shaderldata->programID, 0, "shadowMap");
								Engine::API::drawMesh(modeldata->parts[i].mesh, render.state);
							}
							});
					}
					
					
			});
		///Engine::API::BindFBO(0);
		///Engine::API::Bind(1, colormap);
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
	std::shared_ptr<Engine::Scene::Scene> ActiveScene;
	unsigned int colormap, colorFBO;
};

class CameraSystem : public System {
public:
	CameraSystem(std::shared_ptr<Engine::DATA::windowData> window, std::shared_ptr<Engine::Scene::Scene> scene) : Window(window), ActiveScene(scene) {}
	void Start() override {
		auto& registry = ActiveScene->Registry();
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
		auto& registry = ActiveScene->Registry();
		auto view = registry.view<TransformComponent, CameraComponent>();
		static bool wasCameraControl = true;
		view.each(
			[this](auto entity,
				TransformComponent& transform,
				CameraComponent& cam) {

					if (cam.isactive) {
						if (!Window->hidecursor && !wasCameraControl) 
						{
							cam.firstMouse = true;
							cam.ismoving = false;
							wasCameraControl = true;

							Warn(Engine::CORE::LogCategory::API, cam.firstMouse ? "true" : "false");
							Warn(Engine::CORE::LogCategory::API, "x", Window->mousepos.x,"y", Window->mousepos.y); 
						}
						else if (Window->hidecursor && wasCameraControl)
						{
							wasCameraControl = false;
							cam.ismoving = true;
							Warn(Engine::CORE::LogCategory::API, "x", Window->mousepos.x, "y", Window->mousepos.y);
						}

						Engine::Event::EventManager::GetInstance().Subscribe("windowresize", [&](void* d) {
							auto* size = static_cast<Engine::DATA::Size*>(d);
							cam.cameradata.AspectRatio = static_cast<float>(size->width) / size->height;
							cam.cameradata.windowWidth = size->width; 
							cam.cameradata.windowHeight = size->height;
							cam.dirty = true;
							});
						
						CamUpdate(cam);

						if (cam.ismoving) {
							UpdateCameraRotation(cam, Window);
							Move(transform, 0.1);
						}

						glm::vec3 direction{};
						direction.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
						direction.y = sin(glm::radians(cam.pitch));
						direction.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));

						transform.forward = glm::normalize(direction);
						transform.right = glm::normalize(glm::cross(transform.forward, transform.up)); 

						cam.cameradata.view = glm::lookAt(transform.position, transform.position + transform.forward, transform.up);

						Engine::API::UpdateBuffer(cam.UBO_ID, glm::value_ptr(cam.cameradata.projection), 0, sizeof(glm::mat4));
						Engine::API::UpdateBuffer(cam.UBO_ID, glm::value_ptr(cam.cameradata.view), sizeof(glm::mat4), sizeof(glm::mat4));
						Engine::API::UpdateBuffer(cam.UBO_ID, glm::value_ptr(transform.position), 2 * sizeof(glm::mat4), sizeof(glm::vec4));

						
					}
			});
	}
private:
	std::shared_ptr<Engine::DATA::windowData> Window;
	void UpdateCameraRotation(CameraComponent& cam, std::shared_ptr<Engine::DATA::windowData> window) {

		static float lastX = window->display.width/2.f, lastY = window->display.height / 2.f;

		if (cam.firstMouse)
		{
			lastX = window->mousepos.x;
			lastY = window->mousepos.y;
			cam.firstMouse = false;
			return;
		}
		float xoffset = window->mousepos.x - lastX; 
		float yoffset = lastY - window->mousepos.y;
		xoffset *= cam.sensitivity;
		yoffset *= cam.sensitivity;
		cam.yaw += xoffset;
		cam.pitch += yoffset;
		if (cam.pitch > 89.0f)
			cam.pitch = 89.0f;
		if (cam.pitch < -89.0f)
			cam.pitch = -89.0f;
		lastX = window->mousepos.x; 
		lastY = window->mousepos.y; 
	}
	void CamUpdate(CameraComponent& cam) {
		if(cam.dirty){
			switch (cam.type)
			{
			case ProjectionType::Perspective:
				cam.cameradata.projection = glm::perspective(glm::radians(cam.cameradata.fov),
					cam.cameradata.AspectRatio, cam.cameradata.NCP, cam.cameradata.FCP);
				break;
			case ProjectionType::Orthographic:
				cam.cameradata.projection = glm::ortho(0.0f, static_cast<float>(cam.cameradata.windowWidth),
					0.0f, static_cast<float>(cam.cameradata.windowHeight), -1.0f, 1.0f);
				break;
			default:
				break;
			}
		}
	}
	void Move(TransformComponent& transform, float dt) {

		float velocity = 0.5 * dt;
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
	std::shared_ptr<Engine::Scene::Scene> ActiveScene;
};

class LightSystem : public System {
public:
	LightSystem(std::shared_ptr<Engine::DATA::windowData> window, std::shared_ptr<Engine::Scene::Scene> scene) : Window(window), ActiveScene(scene) {}
	void Start() override {
		auto& registry = ActiveScene->Registry();
		auto view = registry.view<TransformComponent, LightComponent>(); 

		view.each(
			[this](auto entity,
				TransformComponent& transform,
				LightComponent& light) {
					light.UBO_ID = Engine::API::createUBO(112);
					Engine::API::BindBuffer(light.UBO_ID, 1, 0, 112);

					light.dephtmao = Engine::API::createdepthmap(); 
					light.depthFBO = Engine::API::createFBO(light.dephtmao, GL_DEPTH_ATTACHMENT);

					depthshaderID =
						Engine::AssetManager::GetInstance().LoadShader("Shader/depth_shader/vertex_depth_shader.glsl",
																		"Shader/depth_shader/fragment_depth_shader.glsl"); 

					
			});
	}
	void Update(float dt) override {
		auto& registry = ActiveScene->Registry();
		auto view = registry.view<TransformComponent, LightComponent>(); 

		view.each(
			[this, &registry](auto entity,
				TransformComponent& transform,
				LightComponent& light) {
					Engine::API::setViewport(0, 0, Engine::API::SHADOW_WIDTH, Engine::API::SHADOW_HEIGHT);

					glm::vec3 center(0.0);
					auto viewl = registry.view<TransformComponent, CameraComponent>();
					viewl.each(
						[this, &center](auto entity,
							TransformComponent& transform,
							CameraComponent& cam) {
								//glm::vec3 displaycenter(cam.cameradata.windowWidth / 2, cam.cameradata.windowHeight / 2); 
								//if (cam.ismoving) todo : cam active 
								center.x += transform.position.x/10;
								center.z += transform.position.z/10;
						});

					glm::vec3 lightPos = transform.position;
					if (light.type == 0)
						glm::vec3 lightPos = center - transform.position * 20.0f;
					//Warn(Engine::CORE::LogCategory::API, lightPos.x, lightPos.y, lightPos.z); 
					glm::mat4 lightView = glm::lookAt(lightPos, center, transform.up);
					glm::mat4 lightProjection;
					if (light.type == 0) 
						 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 200.0f);
					else
						lightProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 200.0f); 
					light.lightSpaceMatrix = lightProjection * lightView; 


					Engine::API::UpdateBuffer(light.UBO_ID, glm::value_ptr(light.light.lightcolor), 0, sizeof(glm::vec4)); 
					Engine::API::UpdateBuffer(light.UBO_ID, glm::value_ptr(transform.position), sizeof(glm::vec4), sizeof(glm::vec4)); 
					Engine::API::UpdateBuffer(light.UBO_ID, glm::value_ptr(light.lightSpaceMatrix), 2*sizeof(glm::vec4), sizeof(glm::mat4));
					Engine::API::UpdateBuffer(light.UBO_ID, &light.type, 2 * sizeof(glm::vec4) + sizeof(glm::mat4), sizeof(int));

					Engine::API::BindFBO(light.depthFBO);
					Engine::API::clearBuffers(Engine::API::Buffers::DEPTH);
					if(light.shadow) {
						auto viewr = registry.view<TransformComponent, MeshRendererComponent>();
						viewr.each(
							[this, light](auto entity,
								TransformComponent& transform,
								MeshRendererComponent& render) {
									auto modeldata = Engine::AssetManager::GetInstance().GetModel(render.modelhandle);
									auto shaderldata = Engine::AssetManager::GetInstance().GetShader(depthshaderID);
									Engine::API::useShader(*shaderldata);
									if (modeldata and render.state.shadow)
										modeldata->root->each([&](Engine::DATA::Node& node, int depth) {
										Engine::API::SetUniform(shaderldata->programID, transform.modelMatrix * node.localTransform, "model"); 
										Engine::API::SetUniform(shaderldata->programID, light.lightSpaceMatrix, "lightSpaceMatrix");
										for (const int& i : node.meshIndices) {
											Engine::API::drawMesh(modeldata->parts[i].mesh, render.state);
										}

											});
							});
					}
					Engine::API::BindFBO(0);
					Engine::API::Bind(0, light.dephtmao);
					Engine::API::setViewport(0, 0, Window->display.width, Window->display.height);
			});
	}
private:
	Engine::DATA::ShaderHandle depthshaderID;
	std::shared_ptr<Engine::Scene::Scene> ActiveScene; 
	std::shared_ptr<Engine::DATA::windowData> Window;
};

