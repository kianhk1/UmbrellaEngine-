#pragma once
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "WindowAPI.h"
#include "../../../Core/Log Managment/Logger.h"

namespace Engine {
	namespace API {
        struct window {
            GLFWwindow* Window;

            Point position;                     // Window position (required on fullscreen toggle)
            Size display;                       // Display width and height (monitor, device-screen, LCD, ...)
            Size screen;                        // Screen width and height (used render area)
            Size screenMin;                     // Screen minimum width and height (for resizable window)
            Size screenMax;                     // Screen maximum width and height (for resizable window)

            const char* title;                  // Window text title const pointer
            unsigned int flags;                 // Configuration flags (bit based), keeps window state

            bool ready;                         // Check if window has been initialized successfully
            bool shouldClose;                   // Check if window set for closing
            bool hidecursor;
            bool m_isMinimized;
            bool m_isMaximized;
            bool m_isFullscreen;
        };
		class openglWindowAPI : public WindowAPI {
		public:
            openglWindowAPI(){}
			void InitGlfw() {
				if (!glfwInit())
					return;
				int major, minor, rev;
				glfwGetVersion(&major, &minor, &rev);
				Info("GLFW version: " + to_string(major) + "." + to_string(minor) + "." + to_string(rev) + '\n');
			}
            std::shared_ptr<API::window>  InitWindow(int width, int height, const char* title)override {
                std::shared_ptr<API::window> win = std::make_shared<window>();
                win->display.height = height;
                win->display.width = width;
                InitGlfw();
                glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
                glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
                win->Window = glfwCreateWindow(width, height, title, NULL, NULL);
                glfwGetWindowPos(win->Window, &win->position.x, &win->position.y);
                glfwGetWindowSize(win->Window, &win->display.width, &win->display.height);
                if (!win->Window) {
                    glfwTerminate();
                    win->ready = false;
                    return nullptr;
                }
                glfwMakeContextCurrent(win->Window);
                win->ready = true;
                UpdateWindowStatus(win);
                return win;
            }
            void CloseWindow(std::shared_ptr<API::window> win) override {
                glfwSetKeyCallback(win->Window, key_callback);
            }
            bool IsWindowShouldClose(std::shared_ptr<API::window> win) override {
                if (!win->Window) return true;
                CloseWindow(win);
                win->shouldClose = glfwWindowShouldClose(win->Window);
                return win->shouldClose;
            }
            void UpdateWindowStatus(std::shared_ptr<API::window> win) {
                if (!win->Window) return;

                GLFWmonitor* currentMonitor = glfwGetWindowMonitor(win->Window);
                win->m_isFullscreen = (currentMonitor != nullptr);

                int isMinimized = glfwGetWindowAttrib(win->Window, GLFW_ICONIFIED);
                int isMaximized = glfwGetWindowAttrib(win->Window, GLFW_MAXIMIZED);

                win->m_isMinimized = (isMinimized == GLFW_TRUE);
                win->m_isMaximized = (isMaximized == GLFW_TRUE);
            }
            
            void SetFullscreen(std::shared_ptr<API::window> win)override {

                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                if (win->m_isFullscreen) {
                    glfwSetWindowMonitor(win->Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
                    win->screen.height = mode->height;
                    win->screen.width = mode->width;
                    glViewport(0, 0, mode->width, mode->height);
                }
                else {
                    glfwSetWindowMonitor(win->Window, nullptr, win->position.x, win->position.y, win->display.width, win->display.height, GL_DONT_CARE);
                    glViewport(0, 0, win->display.width, win->display.height);
                }
            }
            void ToggleFullscreen(std::shared_ptr<API::window> win) override {
                static bool wasPressed = false;
                bool isPressed = glfwGetKey(win->Window, GLFW_KEY_F11) == GLFW_PRESS;
                if (isPressed && !wasPressed) {
                    SetFullscreen(win);
                    Logger::INFO(win->m_isFullscreen ? "0\n" : "1\n");
                }
                wasPressed = isPressed;
            }
            //void ToggleBorderlessWindowed(void)override;                        // Toggle window state: borderless windowed, resizes window to match monitor resolution
            //void MaximizeWindow(void)override;                                  // Set window state: maximized, if resizable
            //void MinimizeWindow(void)override;                                  // Set window state: minimized, if resizable
            //void RestoreWindow(void)override;                                   // Set window state: not minimized/maximized
            void SetWindowIcon(std::shared_ptr<API::window> win, string imagePath) override {
                GLFWimage images;
                images.pixels = stbi_load(imagePath.c_str(), &images.width, &images.height, 0, 4);
                if (images.pixels) {
                    glfwSetWindowIcon(win->Window, 1, &images);
                    stbi_image_free(images.pixels);
                }
            }
            void ShowCursor(std::shared_ptr<API::window> win) override { glfwSetInputMode(win->Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); win->hidecursor = false; }                                      /// Shows cursor
            void HideCursor(std::shared_ptr<API::window> win) override { glfwSetInputMode(win->Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); win->hidecursor = true; }                                      /// Hides cursor
            //void EnableCursor(void)override;                                    /// Enables cursor (unlock cursor)
            //void DisableCursor(void)override;                                   /// Disables cursor (lock cursor)
            bool IsCursorOnScreen(std::shared_ptr<API::window> win)override {
                if (!win->Window) return false;
                double xpos, ypos;
                glfwGetCursorPos(win->Window, &xpos, &ypos);

                int width, height;
                glfwGetWindowSize(win->Window, &width, &height);

                // اطمینان از اینکه موس در محدوده پنجره است
                return (xpos >= 0 && xpos < width && ypos >= 0 && ypos < height);
            }
            bool GetKey(std::shared_ptr<API::window> win, int keyname, int mode) override {
                return glfwGetKey(win->Window, keyname) == mode;
            }
            Size* GetSize(std::shared_ptr<API::window> win) override {
                if (!win->m_isFullscreen)
                    return &win->display;
                return &win->screen;
            }


            void update(std::shared_ptr<API::window> win) override {
                glfwSwapInterval(1);
                glfwSwapBuffers(win->Window);
                glfwPollEvents();
            }

            static void key_callback(GLFWwindow* Window, int key, int scancode, int action, int mods) {
                if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                    glfwSetWindowShouldClose(Window, GLFW_TRUE);
            }
		private:

		};
	}
}