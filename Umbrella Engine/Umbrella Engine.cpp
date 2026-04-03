#include "Umbrella Engine.h"

std::shared_ptr<API::GraphicsAPI> UE = std::make_unique<API::openglAPI>();
DATA::Texture2DData t_data;

System _system;
int main() {
	UE->InitWindow(800, 600, "Umbrella Engine");
	UE->initialize();
	t_data.t_path = "Assets/as.jpg";
	t_data.t_unit = GL_TEXTURE0;
	obj* v = new obj(UE, &t_data);
	//v->GetComponent<TransformComponent>()->rotation = glm::vec3(0.0, 0.0, glm::radians(90.0));
	_system.AddEntity(v);
	v->Start();

	Entity* cam = new Entity();
	Component* c_trans = new TransformComponent(UE);
	cam->AddComponent(c_trans);
	Component* c_cam = new CameraComponent(UE, ProjectionType::Perspective, 800 / 600, cam->GetComponent<TransformComponent>());
	cam->AddComponent(c_cam);
	Component* aaa = new aa(UE);
	cam->AddComponent(aaa);
	cam->GetComponent<TransformComponent>()->position = glm::vec3(-3.0, 0.0, 0.0);
	_system.AddEntity(cam);
	_system.Start();
	glEnable(GL_DEPTH_TEST);
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
		while (!UE->IsWindowShouldClose())
		{
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			Logger::WARN(to_string(deltaTime));
			float velocity = 2 * deltaTime;

			UE->clearBuffers();
			UE->clearColor(0.8f, 0.8f, 0.8f, 1.0f);
			cam->Update(0.0f);
			v->Update(0.0f);
			_system.Update();
			_system.Move(deltaTime);
			_system.Render();

			UE->update();
		}
}
