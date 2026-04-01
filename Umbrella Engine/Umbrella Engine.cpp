#include "Umbrella Engine.h"

std::shared_ptr<API::GraphicsAPI> UE = std::make_unique<API::openglAPI>();
Texture2DData t_data;
World world;
System _system;
int main() {
	UE->InitWindow(800, 600, "Umbrella Engine");
	UE->initialize();
	t_data.t_path = "Assets/as.jpg";
	obj* v = new obj(UE, &t_data);
	world.AddEntity(v);
	v->Start();

	while (!UE->IsWindowShouldClose())
	{
		UE->clearBuffers();
		UE->clearColor(0.8f, 0.8f, 0.8f, 1.0f);
		v->Update(0.0f);
		_system.Render(world);

		UE->update();
	}
}
