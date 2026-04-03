#pragma once
#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "GraphicsAPI.h"


namespace API {
    class openglAPI : public GraphicsAPI {
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
        //void ToggleBorderlessWindowed(void)override;                        // Toggle window state: borderless windowed, resizes window to match monitor resolution
        //void MaximizeWindow(void)override;                                  // Set window state: maximized, if resizable
        //void MinimizeWindow(void)override;                                  // Set window state: minimized, if resizable
        //void RestoreWindow(void)override;                                   // Set window state: not minimized/maximized
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
       //void EnableCursor(void)override;                                    /// Enables cursor (unlock cursor)
       // void DisableCursor(void)override;                                   /// Disables cursor (lock cursor)
        bool IsCursorOnScreen(void)override {
            if (!Window) return false;
            double xpos, ypos;
            glfwGetCursorPos(Window, &xpos, &ypos);

            int width, height;
            glfwGetWindowSize(Window, &width, &height);

            // اطمینان از اینکه موس در محدوده پنجره است
            return (xpos >= 0 && xpos < width && ypos >= 0 && ypos < height);
        }
        bool GetKey(int keyname, int mode) override {
            return glfwGetKey(Window, keyname) == mode;
        }
        Size GetSize() override {
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
        DATA::ShaderData createShader(const std::string& path_Vertex_Shader, const std::string& path_Fragment_Shader) override {
            unsigned int shaderID = glCreateProgram();
            unsigned int vsID = compileShader(GL_VERTEX_SHADER, path_Vertex_Shader.c_str());
            unsigned int fsID = compileShader(GL_FRAGMENT_SHADER, path_Fragment_Shader.c_str());
            linkShaderProgram(shaderID, vsID, fsID);
            DATA::ShaderData data;
            data.programID = shaderID;
            return data;
        }
        void deleteShader(DATA::ShaderData& shader) override {
            if (shader.programID != 0) {
                glDeleteProgram(shader.programID);
                shader.programID = 0;
            }
        }
        void useShader(const DATA::ShaderData& shader) override {
            glUseProgram(shader.programID);
        }

        // مدیریت مش‌ها (Vertex/Index data)
        void createVAO(DATA::MeshData* data) {
            glGenVertexArrays(1, &data->vaoID);
            glBindVertexArray(data->vaoID);
        }
        void BindVao(DATA::MeshData* data) {
            glBindVertexArray(data->vaoID);
            glBindBuffer(GL_ARRAY_BUFFER, data->vboID);
            glEnableVertexAttribArray(data->iboID);
        }
        void createVBO(DATA::MeshData* data, float* vertex, int a) {
            glBindVertexArray(data->vaoID);
            glGenBuffers(1, &data->vboID);
            glBindBuffer(GL_ARRAY_BUFFER, data->vboID);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * a, vertex, GL_STATIC_DRAW);
        }
        void createIBO(DATA::MeshData* data, unsigned int* indexes, int a) {
            glBindVertexArray(data->vaoID);
            glGenBuffers(1, &data->iboID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->iboID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * a, indexes, GL_STATIC_DRAW);
        }
        void setAttrib(DATA::MeshData* data, int a, int b, int c, int d) override {
            glBindVertexArray(data->vaoID);
            glEnableVertexAttribArray(a);
            glBindBuffer(GL_ARRAY_BUFFER, data->vboID);
            glVertexAttribPointer(a, b, GL_FLOAT, GL_FALSE, c * sizeof(float), (void*)(d * sizeof(float)));
        }
        DATA::MeshData* createMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices) override {
            DATA::MeshData* buffers = new DATA::MeshData();
            createVAO(buffers);
            createVBO(buffers, vertices.data(), vertices.size());
            createIBO(buffers,indices.data(), vertices.size());
            buffers->indexCount = indices.size();
            return buffers;
        }
        void deleteMesh(DATA::MeshData* buffers) override {
            if (buffers->vaoID != 0) glDeleteVertexArrays(1, &buffers->vaoID);
            if (buffers->vboID != 0) glDeleteBuffers(1, &buffers->vboID);
            if (buffers->iboID != 0) glDeleteBuffers(1, &buffers->iboID);
            buffers->vaoID = buffers->vboID = buffers->iboID = 0;
            buffers->indexCount = 0;
        }
        void drawMesh(DATA::MeshData* buffers) override {
            glBindVertexArray(buffers->vaoID);
            glDrawElements(GL_TRIANGLES, buffers->indexCount, GL_UNSIGNED_INT, 0);
        }
        unsigned int createUBO(long long int size_ptr) override {
            unsigned int UBO;
            glGenBuffers(1, &UBO);
            glBindBuffer(GL_UNIFORM_BUFFER, UBO);
            glBufferData(GL_UNIFORM_BUFFER, size_ptr, NULL, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            return UBO;
        }
        void BindBuffer(unsigned int UBO, int slotNumber, int offset, int size) override {
            glBindBuffer(GL_UNIFORM_BUFFER, UBO);
            glBindBufferRange(GL_UNIFORM_BUFFER, slotNumber, UBO, offset, size);
            //glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }
        void UpdateBuffer(unsigned int UBO, void* data, size_t offset, size_t datasize) override {
            glBindBuffer(GL_UNIFORM_BUFFER, UBO);
            glBufferSubData(GL_UNIFORM_BUFFER, offset, datasize, data);
            //glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

        #define ActiveBuffer(Buffer, code) glBindBuffer(GL_UNIFORM_BUFFER, Buffer); code glBindBuffer(GL_UNIFORM_BUFFER, 0);

        // texture
        void Load2DTexture(DATA::Texture2DData* texturedata) override {
            auto it = texturedata->Texturesid.find(texturedata->t_path);

            if (it != texturedata->Texturesid.end()) {
                texturedata->t_id = it->second;
                return;
            }
            texturedata->Texturesid.emplace(texturedata->t_path, loadimage(texturedata));
        }
        unsigned int loadimage(DATA::Texture2DData* texturedata) {
            glGenTextures(1, &texturedata->t_id);
            glBindTexture(GL_TEXTURE_2D, texturedata->t_id);
            // set the texture wrapping/filtering options (on currently bound texture)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // load and generate the texture
            stbi_set_flip_vertically_on_load(true);
            texturedata->pixels = stbi_load(texturedata->t_path.c_str(), &texturedata->width, &texturedata->height, &texturedata->nrChannels, 0);
            if (texturedata->pixels) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texturedata->width, texturedata->height, 0, texturedata->nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, texturedata->pixels);
                glGenerateMipmap(GL_TEXTURE_2D);
                Logger::INFO("Texture loaded successfully: " + texturedata->t_path + " (" + to_string(texturedata->width) + "x" + to_string(texturedata->height) + ")\n");
                stbi_image_free(texturedata->pixels);
            }
            else {
                Logger::ERROR("Texture loaded unsuccessfully: " + texturedata->t_path + " (" + to_string(texturedata->width) + "x" + to_string(texturedata->height) + ")\n" + stbi_failure_reason() + "\n");
                stbi_image_free(texturedata->pixels);
            }
            return texturedata->t_id;
        }
        void Bind(DATA::Texture2DData* texturedata) override {
            glActiveTexture(texturedata->t_unit);
            glBindTexture(GL_TEXTURE_2D, texturedata->t_id);
        }

        //
        void set_mat4(unsigned int shaderID, glm::mat4 setmat4, const GLchar* name)override {
            int loc = glGetUniformLocation(shaderID, name);
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(setmat4));
        }

        void set_vec3(unsigned int shaderID, glm::vec3 setvec3, const GLchar* name)override {
            int loc = glGetUniformLocation(shaderID, name);
            glUniform3fv(loc, 1, glm::value_ptr(setvec3));
        }

        void set_float(unsigned int shaderID, float setfloat, const GLchar* name)override {
            int loc = glGetUniformLocation(shaderID, name);
            glUniform1f(loc, setfloat);
        }

        void set_int(unsigned int shaderID, int setint, const GLchar* name)override {
            int loc = glGetUniformLocation(shaderID, name);
            if (loc == -1) {
                std::cout << "Uniform not found!\n";
            }
            glUniform1i(loc, setint);
        }

        void set_bool(unsigned int shaderID, bool setbool, const GLchar* name)override {
            int loc = glGetUniformLocation(shaderID, name);
            glUniform1i(loc, setbool);
        }

        GLFWwindow* getwindow() override {
            return Window;
        }

        // دریافت وضعیت API (مثلاً آیا اولیه شده است)
        void update() {
            glfwSwapInterval(1);
            glfwSwapBuffers(Window);
            glfwPollEvents();
        }
        //bool isInitialized() const override;
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