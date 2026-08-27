#pragma once
#include "../Renderer/ECS/System.h"

namespace Engine {
	namespace Editor {

		class Layer
		{
		public:
			virtual ~Layer() = default;

			virtual void OnAttach() = 0;
			virtual void OnDetach() = 0;

			virtual void OnUpdate(float ts) = 0;
			virtual void OnUIRender() = 0;
		protected:
			std::shared_ptr<Scene::Scene> activescene;
			virtual void onScene() {
				if (activescene != Engine::Scene::SceneManager::GetInstance().GetActivescene())
				{
					//activescene = Engine::Scene::SceneManager::GetInstance().GetActivescene();
					OnAttach();
				}
			}
		};

	}
}
