#include "Umbrella Engine.h"

std::shared_ptr<API::GraphicsAPI> UE = std::make_unique<API::openglAPI>();


int main() {
	UE->InitWindow(800, 600, "Umbrella Engine");
	UE->initialize();
	obj v(UE);
	v.Start();


	while (!UE->IsWindowShouldClose())
	{
		UE->clearBuffers();
		UE->clearColor(0.8f, 0.8f, 0.8f, 1.0f);
		v.Update(0.0f);

		UE->update();
	}
}
