#include "Umbrella Engine.h"

std::shared_ptr<API::GraphicsAPI> UE = std::make_unique<API::openglAPI>();
System _system;
int main() {
	UE->InitWindow(800, 600, "Umbrella Engine");
	UE->initialize();
	UE->SetWindowIcon("Assets/Umbrella.png");
	UE->HideCursor();

	/*Entity* v = new obj(UE, "Assets/as.jpg", "Assets/ns.png", "Assets/bs.png");
	_system.AddEntity(v);
	v->Start();*/

	Entity* v = new object_load(UE, "Assets/backpack.obj");
	_system.AddEntity(v);
	v->Start();

	Entity* sum = new Entity();
	Component* s_trans = new TransformComponent(UE);
	sum->AddComponent(s_trans);
	Component* s_light = new LightComponent(UE, glm::vec3(1.0f, 1.0f, 1.0f));
	sum->AddComponent(s_light);
	sum->GetComponent<TransformComponent>()->position = glm::vec3(3.0, 2.0, 3.0);
	_system.AddEntity(sum);

	Entity* cam = new Camera(UE, ProjectionType::Perspective, 800.0f / 600.0f);
	Component* aaa = new aa(UE);
	cam->AddComponent(aaa);
	cam->GetComponent<TransformComponent>()->position = glm::vec3(-3.0, 0.0, 0.0);
	_system.AddEntity(cam);
	_system.Start();

	glfwSetWindowUserPointer(UE->getwindow(), &cam);
	glfwSetCursorPosCallback(UE->getwindow(), CameraComponent::mouse_callback);

	glEnable(GL_DEPTH_TEST);
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	while (!UE->IsWindowShouldClose())
	{
		UE->clearBuffers();
		UE->clearColor(0.8f, 0.8f, 0.8f, 1.0f);
		UE->ToggleFullscreen();
		cam->GetComponent<CameraComponent>()->cameradata.AspectRatio = float(UE->GetSize()->width) / float(UE->GetSize()->height);
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		float velocity = 2 * deltaTime;

		cam->Update(deltaTime);
		sum->Update(deltaTime);
		v->Update(deltaTime);
		_system.Update(deltaTime);

		UE->update();
	}
}
