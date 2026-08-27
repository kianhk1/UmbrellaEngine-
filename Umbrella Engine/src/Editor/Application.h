#pragma once

#include "../Renderer/ECS/System.h"
#include "ImGui/ImGuiBuild.h"

#include "Layer.h"
#include "../Renderer/API/GraphicsAPI/GraphicsAPI.h"
#include "../Renderer/API/WindowAPI/WindowAPI.h"
#include "../Core/Data/Data.h"

#include <string>
#include <vector>
#include <memory>
#include <functional>



namespace Engine {
	namespace Editor {

		struct ApplicationSpecification
		{
			std::string Name;
			std::shared_ptr <Engine::DATA::windowData> windowapp;
		};

		class Application
		{
		public:
			Application(const ApplicationSpecification& applicationSpecification);
			~Application();

			static Application& Get();

			void Run();
			void SetMenubarCallback(const std::function<void()>& menubarCallback) { m_MenubarCallback = menubarCallback; }

			template<typename T>
			void PushLayer()
			{
				static_assert(std::is_base_of<Layer, T>::value, "Pushed type is not subclass of Layer!");
				m_LayerStack.emplace_back(std::make_shared<T>())->OnAttach();
			}

			void PushLayer(const std::shared_ptr<Layer>& layer) { m_LayerStack.emplace_back(layer); layer->OnAttach(); }

			void Close();

			float GetTime();
		private:
			void Init();
			void Shutdown();
		private:
			ApplicationSpecification m_Specification;
			bool m_Running = false;

			float m_TimeStep = 0.0f;
			float m_FrameTime = 0.0f;
			float m_LastFrameTime = 0.0f;

			std::vector<std::shared_ptr<Layer>> m_LayerStack;
			std::function<void()> m_MenubarCallback;
		};

		// Implemented by CLIENT
		Application* CreateApplication(int argc, char** argv);

	}
}
