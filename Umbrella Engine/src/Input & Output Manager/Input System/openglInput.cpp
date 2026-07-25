#include <GLFW/glfw3.h>
#include "InputSystem.h"

#include "../../Core/Data/Data.h"
struct Engine::DATA::WindowContext {
	GLFWwindow* window;
};
namespace Engine {
	namespace API {
		void Input::InitInput(std::shared_ptr<DATA::windowData> Window)
		{
			keycallback(Window->windowcontext->window);
			mousebuttoncallback(Window->windowcontext->window);
			mouseposcallback(Window->windowcontext->window);
			windowresizecallback(Window->windowcontext->window);
			windowpositioncallback(Window->windowcontext->window);
			windowclosecallback(Window->windowcontext->window);
		}
		bool Input::IsKeyPressed(std::shared_ptr<DATA::windowData> Window, KeyboardKey key) {
			if (Window->keyboardKey[static_cast<int>(key)] == Action::KEY_PRESS or Window->keyboardKey[static_cast<int>(key)] == Action::KEY_REPEAT) {
				return true;
			}

			return false;
		}
		bool Input::IsButtonPressed(std::shared_ptr<DATA::windowData> Window, MouseButton button) {
			if (Window->mouseButton[static_cast<int>(button)] == Action::KEY_PRESS or Window->mouseButton[static_cast<int>(button)] == Action::KEY_REPEAT) {
				return true;
			}
			return false;
		}
		
		void Input::keycallback(GLFWwindow* Window) { 
			glfwSetKeyCallback(Window, [](GLFWwindow* Window, int key, int scancode, int action, int mods) {
				auto* data = static_cast<DATA::windowData*>(glfwGetWindowUserPointer(Window));
				data->keyboardKey[key] = action;
				if (action == Action::KEY_PRESS)
					Event::EventManager::GetInstance().Broadcast("keypress", &key);
				else if (action == Action::KEY_REPEAT)
					Event::EventManager::GetInstance().Broadcast("keyrepeat", &key);
				else
					Event::EventManager::GetInstance().Broadcast("keyrelease", &key);
				});
		}
		void Input::mousebuttoncallback(GLFWwindow* Window) {
			glfwSetMouseButtonCallback(Window, [](GLFWwindow* Window, int key, int action, int mods) {
				auto* data = static_cast<DATA::windowData*>(glfwGetWindowUserPointer(Window));
				data->mouseButton[key] = action;
				if (action == Action::KEY_PRESS)
					Event::EventManager::GetInstance().Broadcast("buttonpress", &key);
				else if (action == Action::KEY_REPEAT)
					Event::EventManager::GetInstance().Broadcast("buttonrepeat", &key);
				else
					Event::EventManager::GetInstance().Broadcast("buttonrelease", &key);
				});
		}

		void Input::mouseposcallback(GLFWwindow* Window) {
			glfwSetCursorPosCallback(Window, [](GLFWwindow* Window, double xpos, double ypos) {
				auto* data = static_cast<DATA::windowData*>(glfwGetWindowUserPointer(Window));
				data->mousepos.x = static_cast<int>(xpos);
				data->mousepos.y = static_cast<int>(ypos);
				Event::EventManager::GetInstance().Broadcast("mousemove", &data->mousepos);
				});
		}
		void Input::windowpositioncallback(GLFWwindow* Window) {
			glfwSetWindowPosCallback(Window, [](GLFWwindow* Window, int xpos, int ypos) {
				auto* data = static_cast<DATA::windowData*>(glfwGetWindowUserPointer(Window));
				data->position.x = xpos;
				data->position.y = ypos;
				Event::EventManager::GetInstance().Broadcast("windowmove", &data->position);
				});
		}

		void Input::windowresizecallback(GLFWwindow* Window) {
			glfwSetWindowSizeCallback(Window, [](GLFWwindow* Window, int width, int height) {
				auto* data = static_cast<DATA::windowData*>(glfwGetWindowUserPointer(Window));
				data->display.width = width;
				data->display.height = height;
				Event::EventManager::GetInstance().Broadcast("windowresize", &data->display);
				});
		}

		void Input::windowclosecallback(GLFWwindow* Window) {
			glfwSetWindowCloseCallback(Window, [](GLFWwindow* Window) {
				auto* data = static_cast<DATA::windowData*>(glfwGetWindowUserPointer(Window));
				Event::EventManager::GetInstance().Broadcast("windowclose", nullptr);
				});
		}
	}
}
