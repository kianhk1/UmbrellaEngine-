#include "Umbrella Engine.h"

std::shared_ptr<API::GraphicsAPI> UE = std::make_unique<API::openglAPI>();
DATA::Texture2DData t_data;
System _system;
int main() {
	UE->InitWindow(800, 600, "Umbrella Engine");
	UE->initialize();
	UE->SetWindowIcon("Assets/Umbrella.png");
	UE->HideCursor();
	t_data.t_path = "Assets/as.jpg";
	t_data.t_unit = GL_TEXTURE0;
	Entity* v = new obj(UE, &t_data);
	_system.AddEntity(v);
	v->Start();

	Entity* cam = new Entity();
	Component* c_trans = new TransformComponent(UE);
	cam->AddComponent(c_trans);
	float ss = 1.3333f;
	Component* c_cam = new CameraComponent(UE, ProjectionType::Perspective, 800.0f/600.0f, cam->GetComponent<TransformComponent>());
	cam->AddComponent(c_cam);
	Component* aaa = new aa(UE);
	cam->AddComponent(aaa);
	cam->GetComponent<TransformComponent>()->position = glm::vec3(-3.0, 0.0, 0.0);
	v->GetComponent<TransformComponent>()->rotation = glm::vec3(0.0, glm::radians(45.0f), 0.0);
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
		v->Update(deltaTime);
		_system.Update(deltaTime);

		UE->update();
	}
}
