#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <vector>
#include "stb_image.h"
#include "GraphicsAPI.h"
#include "../../../Core/Log Managment/Logger.h"
#include "../../Core/mesh.h"
#include "../../Core/MaterialData.h"

namespace API {
    class openglGraphicsAPI : public GraphicsAPI {
    public:
        void InitGlfw() {
            if (!glfwInit())
                return;
            int major, minor, rev;
            glfwGetVersion(&major, &minor, &rev);
            Logger::INFO("GLFW version: " + to_string(major) + "." + to_string(minor) + "." + to_string(rev) + '\n');
        }
        void InitGlad() {
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
                return;
            const GLubyte* version = glGetString(GL_VERSION);
            Logger::INFO("OpenGL Version: " + string(reinterpret_cast<const char*>(version)) + '\n');

        }
        void InitWindow(int width, int height, const char* title)override {
            display.height = height;
            display.width = width;
            InitGlfw();
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
            Window = glfwCreateWindow(width, height, title, NULL, NULL);
            glfwGetWindowPos(Window, &position.x, &position.y);
            glfwGetWindowSize(Window, &display.width, &display.height);
            if (!Window) {
                glfwTerminate();
                ready = false;
                return;
            }
            glfwMakeContextCurrent(Window);
            ready = true;
            UpdateWindowStatus();
        }
        void InitDebugOutPut(GLDEBUGPROC callback) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(callback, nullptr);
        }
        void CloseWindow(void)override {
            glfwSetKeyCallback(Window, key_callback);
        }
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        bool IsWindowShouldClose(void)override {
            if (!Window) return true;
            CloseWindow();
            shouldClose = glfwWindowShouldClose(Window);
            return shouldClose;
        }

        void UpdateWindowStatus() {
            if (!Window) return;

            GLFWmonitor* currentMonitor = glfwGetWindowMonitor(Window);
            m_isFullscreen = (currentMonitor != nullptr);

            int isMinimized = glfwGetWindowAttrib(Window, GLFW_ICONIFIED);
            int isMaximized = glfwGetWindowAttrib(Window, GLFW_MAXIMIZED);

            m_isMinimized = (isMinimized == GLFW_TRUE);
            m_isMaximized = (isMaximized == GLFW_TRUE);
        }

        bool IsWindowReady()override { return ready; }
        bool IsWindowFullscreen(void)override { return m_isFullscreen; }
        bool IsWindowMinimized(void)override { return m_isMinimized; }
        bool IsWindowMaximized(void) override { return m_isMaximized; }
        bool IsDebugOn() { return usingdebugoutput; }

        void SetFullscreen(bool fullscreen)override {
            m_isFullscreen = fullscreen;

            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);

            if (m_isFullscreen) {
                glfwSetWindowMonitor(Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
                glViewport(0, 0, mode->width, mode->height);
            }
            else {
                glfwSetWindowMonitor(Window, nullptr, position.x, position.y, display.width, display.height, GL_DONT_CARE);
                glViewport(0, 0, display.width, display.height);
            }
        }
        void ToggleFullscreen()override {
            static bool wasPressed = false;
            bool isPressed = glfwGetKey(Window, GLFW_KEY_F11) == GLFW_PRESS;
            if (isPressed && !wasPressed) {
                SetFullscreen(!m_isFullscreen);
                Logger::INFO(m_isFullscreen ? "0\n" : "1\n");
            }
            wasPressed = isPressed;
        }
        void ToggleBorderlessWindowed(void)override;                        // Toggle window state: borderless windowed, resizes window to match monitor resolution
        void MaximizeWindow(void)override;                                  // Set window state: maximized, if resizable
        void MinimizeWindow(void)override;                                  // Set window state: minimized, if resizable
        void RestoreWindow(void)override;                                   // Set window state: not minimized/maximized
        void SetWindowIcon(string imagePath)override {
            GLFWimage images;
            images.pixels = stbi_load(imagePath.c_str(), &images.width, &images.height, 0, 4);
            if (images.pixels) {
                glfwSetWindowIcon(Window, 1, &images);
                stbi_image_free(images.pixels);
            }
        }
        void DebugTurnOn() { usingdebugoutput = true; }
        void DebugTurnOff() { usingdebugoutput = false; }

        void ShowCursor(void) override { glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); hidecursor = false; }                                      /// Shows cursor
        void HideCursor(void) override { glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); hidecursor = true; }                                      /// Hides cursor
        bool IsCursorHidden(void) override { return hidecursor; }                                  /// Check if cursor is not visible
        void EnableCursor(void)override;                                    /// Enables cursor (unlock cursor)
        void DisableCursor(void)override;                                   /// Disables cursor (lock cursor)
        bool IsCursorOnScreen(void)override {
            if (!Window) return false;
            double xpos, ypos;
            glfwGetCursorPos(Window, &xpos, &ypos);

            int width, height;
            glfwGetWindowSize(Window, &width, &height);

            // اطمینان از اینکه موس در محدوده پنجره است
            return (xpos >= 0 && xpos < width && ypos >= 0 && ypos < height);
        }
        Size GetSize()override {
            return display;
        }

        // مقداردهی اولیه API
        bool initialize() override {
            InitGlad();
            if (usingdebugoutput) {
                InitDebugOutPut(Logger::error_callback);
            }
            m_initialized = true;
            return true;
        }

        // پاکسازی منابع
        void shutdown() override {
            if (!m_initialized) return;
            // اینجا می‌توان منابع گلوبال OpenGL را پاک کرد، اگر وجود داشته باشد
            std::cout << "OpenGL Graphics API Shutting Down." << std::endl;
            m_initialized = false;
        }

        void setViewport(int x, int y, int width, int height) override {
            glViewport(x, y, width, height);
        }
        void clearColor(float r, float g, float b, float a) override {
            glClearColor(r, g, b, a);
        }

        // پاک کردن بافر رنگ و عمق
        void clearBuffers() override {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        }

        // مدیریت شیدرها
        ShaderData createShader(const std::string& path_Vertex_Shader, const std::string& path_Fragment_Shader) override {
            unsigned int shaderID = glCreateProgram();
            unsigned int vsID = compileShader(GL_VERTEX_SHADER, path_Vertex_Shader.c_str());
            unsigned int fsID = compileShader(GL_FRAGMENT_SHADER, path_Fragment_Shader.c_str());
            linkShaderProgram(shaderID, vsID, fsID);
            ShaderData data;
            data.programID = shaderID;
            return data;
        }
        void deleteShader(ShaderData& shader) override {
            if (shader.programID != 0) {
                glDeleteProgram(shader.programID);
                shader.programID = 0;
            }
        }
        void useShader(const ShaderData& shader) override {
            glUseProgram(shader.programID);
        }

        // مدیریت مش‌ها (Vertex/Index data)
        Mesh* createMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices, FuncPtr Attrib) override {
            Mesh* buffers = new Mesh;
            buffers->createVAO();
            buffers->createVBO(vertices.data(), vertices.size());
            Attrib();
            return buffers;
        }
        void deleteMesh(Mesh* buffers) override {
            buffers->~Mesh();
        }
        void drawMesh(Mesh* buffers) override {
            buffers->draw(buffers->GetindexCount());
        }

        //
        void set_mat4(unsigned int shaderID, glm::mat4 setmat4, const GLchar* name) {
            int loc = glGetUniformLocation(shaderID, name);
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(setmat4));
        }

        void set_vec3(unsigned int shaderID, glm::vec3 setvec3, const GLchar* name) {
            int loc = glGetUniformLocation(shaderID, name);
            glUniform3fv(loc, 1, glm::value_ptr(setvec3));
        }

        void set_float(unsigned int shaderID, float setfloat, const GLchar* name) {
            int loc = glGetUniformLocation(shaderID, name);
            glUniform1f(loc, setfloat);
        }

        void set_int(unsigned int shaderID, int setint, const GLchar* name) {
            int loc = glGetUniformLocation(shaderID, name);
            glUniform1i(loc, setint);
        }


        // دریافت وضعیت API (مثلاً آیا اولیه شده است)
        bool isInitialized() const override;
        GLFWwindow* Window;
    private:
        unsigned int compileShader(unsigned int type, const char* path) {

            unsigned int shaderID = glCreateShader(type);
            std::string source = Read_File(path);
            const char* src = source.c_str();
            glShaderSource(shaderID, 1, &src, nullptr);
            glCompileShader(shaderID);
            int success;
            char infoLog[512];
            glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
                std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
                return -1;
            }
            return shaderID;
        }
        bool m_initialized = false;
        // تابع کمکی برای لینک کردن شیدرها
        bool linkShaderProgram(GLuint programID, GLuint vertexShaderID, GLuint fragmentShaderID) {
            glAttachShader(programID, vertexShaderID);
            glAttachShader(programID, fragmentShaderID);
            glLinkProgram(programID);

            int success;
            char infoLog[512];
            glGetProgramiv(programID, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(programID, 512, nullptr, infoLog);
                std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
                return false;
            }
            glDeleteShader(vertexShaderID);
            glDeleteShader(fragmentShaderID);
            return true;
        }

        const char* title;                  // Window text title const pointer
        unsigned int flags;                 // Configuration flags (bit based), keeps window state
        bool ready;                         // Check if window has been initialized successfully
        bool shouldClose;                   // Check if window set for closing
        bool hidecursor;
        bool usingdebugoutput = true;

        bool m_isMinimized;
        bool m_isMaximized;
        bool m_isFullscreen;

        Point position;                     // Window position (required on fullscreen toggle)
        Size display;                       // Display width and height (monitor, device-screen, LCD, ...)
        Size screen;                        // Screen width and height (used render area)
        Size screenMin;                     // Screen minimum width and height (for resizable window)
        Size screenMax;                     // Screen maximum width and height (for resizable window)

    };
}