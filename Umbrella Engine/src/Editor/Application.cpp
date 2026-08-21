#include "Application.h"

//
// Adapted from Dear ImGui Vulkan example
//

#include <stdio.h>          // printf, fprintf
#include <stdlib.h>         // abort
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_OPENGL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include <iostream>

// Emedded font
#include "ImGui/Roboto-Regular.embed"
#include "../Input & Output Manager/Input System/InputSystem.h"

extern bool g_ApplicationRunning;
static int                      g_MinImageCount = 2;
static bool                     g_SwapChainRebuild = false;

// Unlike g_MainWindowData.FrameIndex, this is not the the swapchain image index
// and is always guaranteed to increase (eg. 0, 1, 2, 0, 1, 2)
static uint32_t s_CurrentFrameIndex = 0;

static Engine::Editor::Application* s_Instance = nullptr;

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

//#define IMGUI_UNLIMITED_FRAME_RATE
#ifdef _DEBUG
#define IMGUI_VULKAN_DEBUG_REPORT
#endif
#ifdef opengl
struct Engine::DATA::WindowContext {
	GLFWwindow* window;
};
#endif // opengl



namespace Engine {
	namespace Editor {

		Application::Application(const ApplicationSpecification& specification)
			: m_Specification(specification) {
			s_Instance = this;

			Init();
		}

		Application::~Application() {
			Shutdown();

			s_Instance = nullptr;
		}

		Application& Application::Get(){
			return *s_Instance;
		}

		void Application::Init()
		{
			std::cout.rdbuf(&CORE::Logger::consolebuffer);

			
			// Setup GLFW window
			std::shared_ptr <Engine::DATA::windowData> win = m_Specification.windowapp;
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
			m_Specification.scene = Engine::Scene::SceneManager::GetInstance().Creat("scene1");
			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext(); 
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
			//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows 
			//io.ConfigViewportsNoAutoMerge = true;
			//io.ConfigViewportsNoTaskBarIcon = true;

			// Setup Dear ImGui style
			ImGui::StyleColorsDark(); 

			//ImGui::StyleColorsClassic();

			// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			// Setup Platform/Renderer backends
			ImGui_ImplGlfw_InitForOpenGL(win->windowcontext->window, true);
			ImGui_ImplOpenGL3_Init("#version 430");

			// Load default font
			ImFontConfig fontConfig;
			fontConfig.FontDataOwnedByAtlas = false;
			ImFont* robotoFont = io.Fonts->AddFontFromMemoryTTF((void*)g_RobotoRegular, sizeof(g_RobotoRegular), 20.0f, &fontConfig);
			io.FontDefault = robotoFont;

		}

		void Application::Shutdown()
		{
			for (auto& layer : m_LayerStack)
				layer->OnDetach();

			m_LayerStack.clear();

			// Free resources in queue
			API::shutdown();


			g_ApplicationRunning = false;
		}

		void Application::Run()
		{
			std::shared_ptr <Engine::DATA::windowData> win = m_Specification.windowapp;
			std::shared_ptr <Scene::Scene> scene = m_Specification.scene;

			

			RenderSystem sys(scene);
			CameraSystem ccc(win, scene);
			LightSystem ll(win, scene);
			m_Running = true;

			ImGuiIO& io = ImGui::GetIO();

			{
				sys.Start();
				ccc.Start();
				ll.Start();
			}
			// Main loop
			while (!Engine::API::IsWindowShouldClose(win) && m_Running) 
			{
				// Poll and handle events (inputs, window resize, etc.)
				// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
				// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
				// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
				// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
				Engine::API::clearBuffers(7);
				Engine::API::clearColor(0.1f, 0.5f, 0.5f, 1.0f);
				


				{
					ll.Update(0.0f); 
					sys.Update(0.0f); 
					ccc.Update(0.0f);
				}

				if (Engine::API::Input::IsButtonPressed(win, Engine::API::MouseButton::MOUSE_BUTTON_MIDDLE)) {
					Engine::API::HideCursor(win);
				}
				else
					Engine::API::ShowCursor(win); 
				Engine::Event::EventManager::GetInstance().Subscribe("keypress", [&](void* d) {
					auto* key = static_cast<int*>(d);
					if(*key == Engine::API::KeyboardKey::KEY_S and
						(win->keyboardKey[*key].mod and API::Mod::CONTROL))
						Event::EventManager::GetInstance().Broadcast("savescene", &scene);
					});
				
				Engine::Event::EventManager::GetInstance().Subscribe("savescene", [&](void* d) {
					auto* scene =
						static_cast<std::shared_ptr<Engine::Scene::Scene>*>(d);
					Engine::Scene::SceneManager::GetInstance().Save(*scene);  
					Info(Engine::CORE::LogCategory::API, "save scene:'", scene->get()->GetName(), "'");
					});

				for (auto& layer : m_LayerStack)
					layer->OnUpdate(m_TimeStep);


				// Start the Dear ImGui frame
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				{
					static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

					// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
					// because it would be confusing to have two docking targets within each others.
					ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
					if (m_MenubarCallback)
						window_flags |= ImGuiWindowFlags_MenuBar;

					const ImGuiViewport* viewport = ImGui::GetMainViewport();
					ImGui::SetNextWindowPos(viewport->WorkPos);
					ImGui::SetNextWindowSize(viewport->WorkSize);
					ImGui::SetNextWindowViewport(viewport->ID);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
					window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
					window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

					// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
					// and handle the pass-thru hole, so we ask Begin() to not render a background.
					if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
						window_flags |= ImGuiWindowFlags_NoBackground;

					// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
					// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
					// all active windows docked into it will lose their parent and become undocked.
					// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
					// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
					ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
					ImGui::Begin("DockSpace Demo", nullptr, window_flags);
					ImGui::PopStyleVar();

					ImGui::PopStyleVar(2);

					// Submit the DockSpace
					ImGuiIO& io = ImGui::GetIO();
					if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
					{
						ImGuiID dockspace_id = ImGui::GetID("Umrella Engine");
						ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
					}

					if (m_MenubarCallback)
					{
						if (ImGui::BeginMenuBar())
						{
							m_MenubarCallback();
							ImGui::EndMenuBar();
						}
					}

					for (auto& layer : m_LayerStack)
						layer->OnUIRender();

					ImGui::End();
				}

				// Rendering
				ImGui::Render();

				ImDrawData* draw_data = ImGui::GetDrawData();

				if (draw_data->DisplaySize.x > 0.0f &&
					draw_data->DisplaySize.y > 0.0f)
				{
					ImGui_ImplOpenGL3_RenderDrawData(draw_data);
				}

				// Additional Platform Windows
				if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					ImGui::UpdatePlatformWindows();
					ImGui::RenderPlatformWindowsDefault();
				}

				

				API::update(win);

				float time = GetTime();
				m_FrameTime = time - m_LastFrameTime;
				m_TimeStep = glm::min<float>(m_FrameTime, 0.0333f);
				m_LastFrameTime = time;


				if (Engine::API::Input::IsKeyPressed(win, Engine::API::KeyboardKey::KEY_ESCAPE)) {
					Engine::API::CloseWindow(win);
				}
				Engine::Event::EventManager::GetInstance().Clear();
			}

		}

		void Application::Close()
		{
			m_Running = false;
		}

		float Application::GetTime()
		{
			return (float)glfwGetTime();
		}

		
	}
}

