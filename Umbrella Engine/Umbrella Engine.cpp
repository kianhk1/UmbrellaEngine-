#include "src/Renderer/ECS/System.h"
#include "src/Renderer/API/WindowAPI/WindowAPI.h"
#include <iostream>

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
		registry.emplace<TransformComponent>(entity,t);
		registry.emplace<MeshRendererComponent>(entity,m);
	}
	ObjCube(std::shared_ptr<Engine::Scene::Scene> scene, int x, int y , int z) {
		auto& registry = scene->Registry();
		entity = registry.create();
		TransformComponent t;
		t.position = glm::vec3(x,y,z);
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

int main() {
	std::shared_ptr <Engine::DATA::windowData> win = Engine::API::InitWindow(800, 600, "Umbrella Engine");
	Engine::API::SetWindowIcon(win, "Assets/Umbrella.png");
	Engine::API::initialize();
	Engine::API::Input::InitInput(win);
	std::thread([win]() {
		while (!Engine::API::IsWindowShouldClose(win))
		{
			Engine::CORE::Logger::Flush();

			std::this_thread::sleep_for(
				std::chrono::milliseconds(10));
		}
		}).detach();

	Engine::API::HideCursor(win);

	auto scene = Engine::Scene::SceneManager::GetInstance().Creat("scene1");
	Engine::Scene::SceneManager::GetInstance().Load(scene);

	ObjCube sss(scene, "Assets/skybox/skybox.json", 5);
	ObjCube cccc(scene, "Assets/chess.gltf");
	//ObjCube dddd("Assets/TEST.gltf");
	ObjCube nn(scene, 1, 1, 1);

	Entity camera{ scene->Registry().create()};
	CameraComponent cam;
	TransformComponent trans;
	cam.type = ProjectionType::Perspective;
	
	trans.position = glm::vec3(1.0f);
	scene->Registry().emplace<CameraComponent>(camera, cam);
	scene->Registry().emplace<TransformComponent>(camera, trans);
	
	RenderSystem sys(scene);
	CameraSystem ccc(win, scene);
	LightSystem ll(scene);
	sys.Start(); 
	ccc.Start();
	ll.Start();
	int frameCount = 0;
	
	auto fil = Engine::Scene::SceneManager::GetInstance().Save();
	//Warn(Engine::CORE::LogCategory::Resource, fil ? "yes": "no");

	while (!Engine::API::IsWindowShouldClose(win))
	{
		Engine::API::clearBuffers(7);
		Engine::API::clearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Engine::Event::EventManager::GetInstance().Subscribe("windowresize", [&](void* d) {
			auto* size = static_cast<Engine::DATA::Size*>(d);
			std::cout << "x:" << size->width << " y:" << size->height << '\n';
			});
		
		if (Engine::API::Input::IsKeyPressed(win, Engine::API::KeyboardKey::KEY_W)) {
			//Info(Engine::CORE::LogCategory::Nune, "Right mouse button pressed.");
		}
		ll.Update(0.0f); 
		sys.Update(0.0f);
		ccc.Update(0.0f);

		Engine::API::update(win);

		frameCount++;
		if (Engine::API::Input::IsKeyPressed(win, Engine::API::KeyboardKey::KEY_ESCAPE)) {
			Engine::API::CloseWindow(win);
		}
		Engine::Event::EventManager::GetInstance().Clear();
	}
	Engine::API::shutdown();
}


