#pragma once
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "WindowAPI.h"
#include "../../../Input & Output Manager/Input System/InputSystem.h"
#include "../../../Core/Log Managment/Logger.h"
#include "../../../Core/Data/Data.h"
struct Engine::DATA::WindowContext {
    GLFWwindow* window;
};
namespace Engine {
	namespace API {
#ifdef opengl
			void InitGlfw() {
                if (!glfwInit())
                {
                    ّFatal(Engine::CORE::LogCategory::API, "initialize Glfw failed.");
                    return;
                }
				int major, minor, rev;
				glfwGetVersion(&major, &minor, &rev);
				Info(Engine::CORE::LogCategory::API, "GLFW version : " + to_string(major) + "." + to_string(minor) + "." + to_string(rev));
			}
            std::shared_ptr<DATA::windowData>  InitWindow(int width, int height, const char* title) {
                std::shared_ptr<DATA::windowData> win = std::make_shared<DATA::windowData>();
                win->windowcontext = std::make_shared<DATA::WindowContext>();
                win->display.height = height; 
                win->display.width = width;
                win->title = title;
                InitGlfw();
                glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
                glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
                win->windowcontext->window = glfwCreateWindow(win->display.width, win->display.height, win->title, NULL, NULL); 
                glfwGetWindowPos(win->windowcontext->window, &win->position.x, &win->position.y);
                glfwGetWindowSize(win->windowcontext->window, &win->display.width, &win->display.height);
                if (!win->windowcontext->window) {
                    ّFatal(Engine::CORE::LogCategory::API, "Creating a new window failed.");
                    glfwTerminate();
                    win->ready = false;
                    return nullptr;
                }
                glfwMakeContextCurrent(win->windowcontext->window);
                glfwSetWindowUserPointer(win->windowcontext->window, win.get());
                Engine::Event::EventManager::GetInstance().Subscribe("windowclose", [&](void* d) {
                    Info(Engine::CORE::LogCategory::API, "The " + string(win->title) + " window closed.");
                    CloseWindow(win);
                    });
                win->ready = true;
                Info(Engine::CORE::LogCategory::API, "The new window was created successfully.");
                UpdateWindowStatus(win);
                return win;
            }
            void CloseWindow(std::shared_ptr<DATA::windowData> win) {
                if (win && win->windowcontext->window) {
                    glfwDestroyWindow(win->windowcontext->window);
                    win->shouldClose = glfwWindowShouldClose(win->windowcontext->window);
                    win->shouldClose = true;
                    win->windowcontext->window = nullptr;
                }
            }
            bool IsWindowShouldClose(std::shared_ptr<DATA::windowData> win) {
                if (win && win->windowcontext->window)
					win->shouldClose = glfwWindowShouldClose(win->windowcontext->window);
                return win->shouldClose;
            }
            void UpdateWindowStatus(std::shared_ptr<DATA::windowData> win) {
                if (!win->windowcontext->window) return;

                GLFWmonitor* currentMonitor = glfwGetWindowMonitor(win->windowcontext->window);
                win->m_isFullscreen = (currentMonitor != nullptr);

                int isMinimized = glfwGetWindowAttrib(win->windowcontext->window, GLFW_ICONIFIED);
                int isMaximized = glfwGetWindowAttrib(win->windowcontext->window, GLFW_MAXIMIZED);

                win->m_isMinimized = (isMinimized == GLFW_TRUE);
                win->m_isMaximized = (isMaximized == GLFW_TRUE);
            }
			void Setviewport(DATA::Size windowsize) {
				glViewport(0, 0, windowsize.width, windowsize.height);
			}
            
            void SetFullscreen(std::shared_ptr<DATA::windowData> win) {

                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                if (win->m_isFullscreen) {
                    glfwSetWindowMonitor(win->windowcontext->window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
                    win->screen.height = mode->height;
                    win->screen.width = mode->width;
                    glViewport(0, 0, mode->width, mode->height);
                }
                else {
                    glfwSetWindowMonitor(win->windowcontext->window, nullptr, win->position.x, win->position.y, win->display.width, win->display.height, GL_DONT_CARE);
                    glViewport(0, 0, win->display.width, win->display.height);
                }
            }
            void ToggleFullscreen(std::shared_ptr<DATA::windowData> win) {
                static bool wasPressed = false;
                bool isPressed = glfwGetKey(win->windowcontext->window, GLFW_KEY_F11) == GLFW_PRESS;
                if (isPressed && !wasPressed) {
                    SetFullscreen(win);
                    Info(Engine::CORE::LogCategory::API, win->m_isFullscreen ? "0\n" : "1\n");
                }
                wasPressed = isPressed;
            }
            //void ToggleBorderlessWindowed(void)override;                        // Toggle window state: borderless windowed, resizes window to match monitor resolution
            //void MaximizeWindow(void)override;                                  // Set window state: maximized, if resizable
            //void MinimizeWindow(void)override;                                  // Set window state: minimized, if resizable
            //void RestoreWindow(void)override;                                   // Set window state: not minimized/maximized
            void SetWindowIcon(std::shared_ptr<DATA::windowData> win, string imagePath) {
                GLFWimage images;
                images.pixels = stbi_load(imagePath.c_str(), &images.width, &images.height, 0, 4);
                if (!images.pixels) {
					Error(Engine::CORE::LogCategory::API, "Failed to load window icon: " + imagePath + "\n" + stbi_failure_reason() + "\n");
					return;
                }
                glfwSetWindowIcon(win->windowcontext->window, 1, &images);
                stbi_image_free(images.pixels);
            }
            void ShowCursor(std::shared_ptr<DATA::windowData> win) { glfwSetInputMode(win->windowcontext->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); win->hidecursor = false; }                                      /// Shows cursor
            void HideCursor(std::shared_ptr<DATA::windowData> win) { glfwSetInputMode(win->windowcontext->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); win->hidecursor = true; }                                      /// Hides cursor
            //void EnableCursor(void);                                    /// Enables cursor (unlock cursor)
            //void DisableCursor(void);                                   /// Disables cursor (lock cursor)
            bool IsCursorOnScreen(std::shared_ptr<DATA::windowData> win) {
                if (!win->windowcontext->window) return false;
                double xpos, ypos;
                glfwGetCursorPos(win->windowcontext->window, &xpos, &ypos);

                int width, height;
                glfwGetWindowSize(win->windowcontext->window, &width, &height);

                // اطمینان از اینکه موس در محدوده پنجره است
                return (xpos >= 0 && xpos < width && ypos >= 0 && ypos < height);
            }
            bool GetKey(std::shared_ptr<DATA::windowData> win, int keyname, int mode) { // پاااااک شود
                return glfwGetKey(win->windowcontext->window, keyname) == mode;
            }
            DATA::Size GetSize(std::shared_ptr<DATA::windowData> win) {// پاااااک شود
                if (!win->m_isFullscreen)
                    return win->display;
                return win->screen;
            }


            void update(std::shared_ptr<DATA::windowData> win) {
                Engine::Event::EventManager::GetInstance().Subscribe("windowresize", [&](void* d) {
                    Setviewport(win->display);
                    });
                glfwSwapInterval(1);
                glfwSwapBuffers(win->windowcontext->window);
                glfwPollEvents();
            }
#endif

	}
}