#include "src/Renderer/ECS/System.h"
#include "src/Renderer/API/WindowAPI/openglWindowAPI.h"
#include <iostream>

std::shared_ptr<Engine::API::GraphicsAPI> GUE;
std::shared_ptr<Engine::API::openglWindowAPI> WUE = std::make_shared<Engine::API::openglWindowAPI>();

class ObjCube
{
public:
	ObjCube() {
		Entity = registry.create();
		TransformComponent t;
		MeshComponent m;
		m.meshdata.vertices = vertex;
		m.meshdata.indices = index;
		ShaderComponent s;
		s.shader.path_Vertex_Shader = "Shader/vvvv.glsl";
		s.shader.path_Fragment_Shader = "Shader/fffff.glsl";
		registry.emplace<TransformComponent>(Entity,t); 
		registry.emplace<MeshComponent>(Entity,m); 
		registry.emplace<ShaderComponent>(Entity,s);
	}
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
	entt::entity Entity;
};



int main() {
	std::shared_ptr < Engine::API::window> win = WUE->InitWindow(800, 600, "Umbrella Engine");
	GUE = Engine::API::openglAPI::creategraphicAPI(); 
	WUE->SetWindowIcon(win, "Assets/Umbrella.png");

	ObjCube cccc;

	RenderSystem sys(GUE);
	sys.Start();
	//registry
	while (!WUE->IsWindowShouldClose(win)) 
	{
		GUE->clearBuffers(); 
		GUE->clearColor(0.8f, 0.8f, 0.8f, 1.0f); 
		sys.Update(0.0f);
		WUE->update(win); 
	}
}