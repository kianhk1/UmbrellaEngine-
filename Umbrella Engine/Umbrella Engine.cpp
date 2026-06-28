#include "src/Renderer/ECS/System.h"
#include "src/Renderer/API/WindowAPI/WindowAPI.h"
#include <iostream>

class ObjCube
{
public:
	ObjCube() {
		Entity = registry.create();
		TransformComponent t;
		MeshComponent m;
		m.meshdata.vertices = vertex;
		m.meshdata.indices = index;
		MatrialComponent ma;
		ma.matrialdata.shader.ID =
			Engine::AssetManager::GetInstance().LoadShader("Shader/vvvv.glsl", "Shader/fffff.glsl").ID;
		ma.matrialdata.textures[0].ID =
			Engine::AssetManager::GetInstance().LoadTexture("Assets/as.jpg").ID;

		registry.emplace<TransformComponent>(Entity,t); 
		registry.emplace<MeshComponent>(Entity,m); 
		registry.emplace<MatrialComponent>(Entity, ma);
	}
	ObjCube(glm::vec3 pos) {
		Entity = registry.create();
		TransformComponent t;
		t.position = pos;
		MeshComponent m;
		m.meshdata.vertices = vertex;
		m.meshdata.indices = index;
		MatrialComponent ma;
		ma.matrialdata.shader.ID =
			Engine::AssetManager::GetInstance().LoadShader("Shader/vvvv.glsl", "Shader/fffff.glsl").ID;
		ma.matrialdata.textures[0].ID =
			Engine::AssetManager::GetInstance().LoadTexture("Assets/as.jpg").ID; 
		Engine::AssetManager::GetInstance().GetTexture(ma.matrialdata.textures[0])->unit = 0;

		registry.emplace<TransformComponent>(Entity, t);
		registry.emplace<MeshComponent>(Entity, m);
		registry.emplace<MatrialComponent>(Entity, ma);
	}
	entt::entity Entity;
private:
	std::vector<float> vertex = {
		//  x,    y,    z,        r,   g,   b,      u,  v,       nx,    ny,    nz,      tx,    ty,    tz
		// جلو (Front) - تانژانت در راستای X
		-0.5f, -0.5f,  0.5f,    1.0, 0.0, 0.0,    0.0, 0.0,    0.0f,  0.0f,  1.0f,    1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,    0.0, 1.0, 0.0,    1.0, 0.0,    0.0f,  0.0f,  1.0f,    1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,    0.0, 0.0, 1.0,    1.0, 1.0,    0.0f,  0.0f,  1.0f,    1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    1.0, 1.0, 0.0,    0.0, 1.0,    0.0f,  0.0f,  1.0f,    1.0f,  0.0f,  0.0f,

		// پشت (Back) - تانژانت در راستای معکوس X (بسته به جهت بافت)
		-0.5f, -0.5f, -0.5f,    1.0, 0.0, 1.0,    0.0, 0.0,    0.0f,  0.0f, -1.0f,   -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,    0.0, 1.0, 1.0,    1.0, 0.0,    0.0f,  0.0f, -1.0f,   -1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,    1.0, 1.0, 1.0,    1.0, 1.0,    0.0f,  0.0f, -1.0f,   -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.2, 0.8, 0.3,    0.0, 1.0,    0.0f,  0.0f, -1.0f,   -1.0f,  0.0f,  0.0f,

		// چپ (Left) - تانژانت در راستای Z
		-0.5f, -0.5f, -0.5f,    1.0, 0.0, 0.0,    0.0, 0.0,   -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0, 1.0, 0.0,    1.0, 0.0,   -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0, 0.0, 1.0,    1.0, 1.0,   -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    1.0, 1.0, 0.0,    0.0, 1.0,   -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,  1.0f,

		// راست (Right) - تانژانت در راستای معکوس Z
		 0.5f, -0.5f, -0.5f,    1.0, 0.0, 1.0,    0.0, 0.0,    1.0f,  0.0f,  0.0f,    0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f,  0.5f,    0.0, 1.0, 1.0,    1.0, 0.0,    1.0f,  0.0f,  0.0f,    0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f,  0.5f,    1.0, 1.0, 1.0,    1.0, 1.0,    1.0f,  0.0f,  0.0f,    0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,    0.2, 0.8, 0.3,    0.0, 1.0,    1.0f,  0.0f,  0.0f,    0.0f,  0.0f, -1.0f,

		 // بالا (Top) - تانژانت در راستای X
		-0.5f,  0.5f, -0.5f,    1.0, 0.0, 0.0,    0.0, 0.0,    0.0f,  1.0f,  0.0f,    1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,    0.0, 1.0, 0.0,    1.0, 0.0,    0.0f,  1.0f,  0.0f,    1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,    0.0, 0.0, 1.0,    1.0, 1.0,    0.0f,  1.0f,  0.0f,    1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    1.0, 1.0, 0.0,    0.0, 1.0,    0.0f,  1.0f,  0.0f,    1.0f,  0.0f,  0.0f,

		// پایین (Bottom) - تانژانت در راستای معکوس X
	   -0.5f, -0.5f, -0.5f,    1.0, 0.0, 1.0,    0.0, 0.0,    0.0f, -1.0f,  0.0f,   -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,    0.0, 1.0, 1.0,    1.0, 0.0,    0.0f, -1.0f,  0.0f,   -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,    1.0, 1.0, 1.0,    1.0, 1.0,    0.0f, -1.0f,  0.0f,   -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,    0.2, 0.8, 0.3,    0.0, 1.0,    0.0f, -1.0f,  0.0f,   -1.0f,  0.0f,  0.0f
	};
	std::vector<unsigned int> index = {
		0, 1, 2, 2, 3, 0,       // جلو
		4, 5, 6, 6, 7, 4,       // پشت
		8, 9,10,10,11, 8,       // چپ
	   12,13,14,14,15,12,       // راست
	   16,17,18,18,19,16,       // بالا
	   20,21,22,22,23,20        // پایین
	};

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

	ObjCube cccc;
	ObjCube nn({ -1,-1,-1 });

	entt::entity camera = registry.create();
	CameraComponent cam;
	TransformComponent trans;
	cam.type = ProjectionType::Perspective;
	
	trans.position = { -2,0,0 };
	registry.emplace<CameraComponent>(camera, cam);
	registry.emplace<TransformComponent>(camera, trans);
	
	RenderSystem sys;
	CameraSystem ccc(win);
	sys.Start();
	ccc.Start();
	int frameCount = 0;
	

	while (!Engine::API::IsWindowShouldClose(win))
	{
		Engine::API::clearBuffers();
		Engine::API::clearColor(0.8f, 0.8f, 0.8f, 1.0f);
		Engine::Event::EventManager::GetInstance().Subscribe("windowresize", [&](void* d) {
			auto* size = static_cast<Engine::DATA::Size*>(d);
			std::cout << "x:" << size->width << " y:" << size->height << '\n';
			});
		
		if (Engine::API::Input::IsKeyPressed(win, Engine::API::KeyboardKey::KEY_W)) {
			Info(Engine::CORE::LogCategory::Nune, "Right mouse button pressed.");
		}

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