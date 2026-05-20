#include "Umbrella Engine.h"
#include "Libraries/imgui/imgui.h"
#include "Libraries/imgui/backends/imgui_impl_glfw.h"
#include "Libraries/imgui/backends/imgui_impl_opengl3.h"


std::shared_ptr<API::GraphicsAPI> UE = std::make_unique<API::openglAPI>();
//Engine::DATA::WorldConfig word;
//std::shared_ptr<Engine::API::Physics::PhysicsWorld> PE = std::make_unique<Engine::API::Physics::BulletPhysicsWorld>(word);

System _system;

// NVIDIA GPU
extern "C" {
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
// AMD GPU
extern "C" {
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int main() {
	UE->InitWindow(800, 600, "Umbrella Engine");
	UE->initialize();
	UE->SetWindowIcon("Assets/Umbrella.png");
	UE->HideCursor();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();


	/*Entity* v = new obj(UE, "Assets/as.jpg", "Assets/ns.png", "Assets/bs.png");
	_system.AddEntity(v);
	v->Start();*/

	object_load* v = new object_load(_system.GetManager(), UE, "Assets/backpack.obj");
	//_system.AddEntity(v); 
	v->Start();

	ObjBase* sum = new ObjBase(_system.GetManager(), UE);
	Component* s_light = new LightComponent(UE, glm::vec3(1.0f, 1.0f, 1.0f));
	sum->AddComponent(s_light);
	sum->GetComponent<TransformComponent>()->position = glm::vec3(3.0, 2.0, 3.0);
	//_system.AddEntity(sum);

	Camera* cam = new Camera(_system.GetManager(), UE, ProjectionType::Perspective, 800.0f / 600.0f);
	Component* aaa = new RigidBodyComponent(UE);
	cam->AddComponent(aaa);
	cam->GetComponent<TransformComponent>()->position = glm::vec3(-3.0, 0.0, 0.0);
	//_system.AddEntity(cam);
	_system.Start();

	glfwSetWindowUserPointer(UE->getwindow(), &cam);
	glfwSetCursorPosCallback(UE->getwindow(), CameraComponent::mouse_callback);

	ImGui_ImplGlfw_InitForOpenGL(UE->getwindow(), true);
	ImGui_ImplOpenGL3_Init("#version 430");

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


		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		//presskey(GLFW_KEY_R, glPolygonMode(GL_FRONT_AND_BACK, f ? GL_FILL : GL_LINE));
		static bool edittmode = true;
		static bool wasPressed = false;
		bool isPressed = UE->GetKey(GLFW_KEY_R, GLFW_PRESS);
		if (isPressed && !wasPressed) {
			if (edittmode) {
				cam->GetComponent<CameraComponent>()->ismoving = false;
				cam->GetComponent<CameraComponent>()->firstMouse = true;
				UE->ShowCursor();
			}
			else {
				cam->GetComponent<CameraComponent>()->ismoving = true;
				cam->GetComponent<CameraComponent>()->isActive = true;
				UE->HideCursor();
			}
			Logger::INFO(edittmode ? "0\n" : "1\n");
			edittmode = !edittmode;
		}
		wasPressed = isPressed;

		_system.Update(deltaTime);
		static Component* selectedComponent = nullptr;
		ImGui::Begin("Debug Panel");
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
		ImGui::Text("Position: %.2f, %.2f, %.2f",
			cam->GetComponent<TransformComponent>()->position.x,
			cam->GetComponent<TransformComponent>()->position.y,
			cam->GetComponent<TransformComponent>()->position.z);

		for (auto& entity : _system.GetManager()->GetAllEntities())
		{
			ImGui::PushID(entity.first);

			if (ImGui::TreeNode((to_string(entity.first)).c_str()))
			{
				for (auto& comp : entity.second)
				{
					ImGui::PushID(comp);

					if (ImGui::TreeNode(comp->name.c_str()))
					{
						if (TransformComponent* trans = dynamic_cast<TransformComponent*>(comp))
						{
							ImGui::InputFloat3("Position", &trans->position.x);

							glm::vec3 euler = glm::degrees(trans->rotation);
							if (ImGui::InputFloat3("Rotation", &euler.x)) {
								trans->rotation = glm::radians(euler);
							}

							ImGui::InputFloat3("Scale", &trans->scale.x);
						}
						if (LightComponent* light = dynamic_cast<LightComponent*>(comp))
						{
							ImGui::ColorPicker3("Color", glm::value_ptr(light->light.lightcolor));
						}

						ImGui::TreePop();
					}

					ImGui::PopID();
				}
				ImGui::TreePop();
			}

			ImGui::PopID();
		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		UE->update();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	//_system.~System();
}
