#pragma once
#pragma once
#include <iostream>
#include <glad/glad.h>
#include <memory>
#include "stb_image.h"
#include "GraphicsAPI.h"


namespace Engine {
    namespace API {
        class openglAPI : public GraphicsAPI {
        public:
            static std::shared_ptr<openglAPI> creategraphicAPI() {
                if (!graphic) {
                    return graphic = std::make_shared<openglAPI>();
                }
                return graphic;
            }
            void InitDebugOutPut(GLDEBUGPROC callback) {
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                glDebugMessageCallback(callback, nullptr);
            }
            bool IsDebugOn() { return usingdebugoutput; }
            void DebugTurnOn() { usingdebugoutput = true; }
            void DebugTurnOff() { usingdebugoutput = false; }

            // مقداردهی اولیه API
            bool initialize() override {
                if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
                    return m_initialized = false;;
                const GLubyte* version = glGetString(GL_VERSION);
                Info("OpenGL Version: " + string(reinterpret_cast<const char*>(version)) + '\n');
                if (usingdebugoutput) {
                    InitDebugOutPut(Logger::error_callback);
                }
                m_initialized = true;
                return m_initialized = true;
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
            unsigned int createShader() override {
                unsigned int programID;
                return programID = glCreateProgram();
            }
            unsigned int compileShader(unsigned int type, const std::string source) override {

                unsigned int shaderID = glCreateShader(type);
                const char* src = source.c_str();
                glShaderSource(shaderID, 1, &src, nullptr);
                glCompileShader(shaderID);
                int success;
                char infoLog[512];
                glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
                if (!success) {
                    glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
                    std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
                    glDeleteProgram(shaderID);
                    return 0;
                }
                return shaderID;
            }
            bool linkShaderProgram(GLuint programID, GLuint vertexShaderID, GLuint fragmentShaderID) override {
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
            void createVAO(DATA::MeshData& data) {
                glGenVertexArrays(1, &data.vaoID);
                glBindVertexArray(data.vaoID);
            }
            void BindVao(DATA::MeshData& data) {
                glBindVertexArray(data.vaoID);
                glBindBuffer(GL_ARRAY_BUFFER, data.vboID);
                glEnableVertexAttribArray(data.iboID);
            }
            void createVBO(DATA::MeshData& data, float* vertex, int a) {
                glBindVertexArray(data.vaoID);
                glGenBuffers(1, &data.vboID);
                glBindBuffer(GL_ARRAY_BUFFER, data.vboID);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * a, vertex, GL_STATIC_DRAW);
            }
            void createIBO(DATA::MeshData& data, unsigned int* indexes, int a) {
                glBindVertexArray(data.vaoID);
                glGenBuffers(1, &data.iboID);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.iboID);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * a, indexes, GL_STATIC_DRAW);
            }
            void setAttrib(DATA::MeshData& data, int a, int b, int c, int d) override {
                glBindVertexArray(data.vaoID);
                glEnableVertexAttribArray(a);
                glBindBuffer(GL_ARRAY_BUFFER, data.vboID);
                glVertexAttribPointer(a, b, GL_FLOAT, GL_FALSE, c * sizeof(float), (void*)(d * sizeof(float)));
            }
            DATA::MeshData createMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices) override {
                DATA::MeshData buffers;
                createVAO(buffers);
                createVBO(buffers, vertices.data(), vertices.size());
                createIBO(buffers, indices.data(), indices.size());
                buffers.indexCount = indices.size();
                return buffers;
            }
            void deleteMesh(DATA::MeshData& buffers) override {
                if (buffers.vaoID != 0) glDeleteVertexArrays(1, &buffers.vaoID);
                if (buffers.vboID != 0) glDeleteBuffers(1, &buffers.vboID);
                if (buffers.iboID != 0) glDeleteBuffers(1, &buffers.iboID);
                buffers.vaoID = buffers.vboID = buffers.iboID = 0;
                buffers.indexCount = 0;
            }
            void drawMesh(DATA::MeshData& buffers) override {
                glBindVertexArray(buffers.vaoID);
                glDrawElements(GL_TRIANGLES, buffers.indexCount, GL_UNSIGNED_INT, 0);
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

#define ActiveBuffer(Buffer, code) glBindBuffer(GL_UNIFORM_BUFFER, Buffer); code glBindBuffer(GL_UNIFORM_BUFFER, 0)

            // texture ...........
            void createtexture(DATA::Texture2DData* texturedata) {
                glGenTextures(1, &texturedata->t_id);
                glBindTexture(GL_TEXTURE_2D, texturedata->t_id);
                // set the texture wrapping/filtering options (on currently bound texture)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }
            void loadtexture(DATA::Texture2DData* texturedata) {
                // load and generate the texture
                stbi_set_flip_vertically_on_load(true);
                texturedata->pixels = stbi_load(texturedata->t_path.c_str(), &texturedata->width, &texturedata->height, &texturedata->nrChannels, 0);
                if (texturedata->pixels) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texturedata->width, texturedata->height, 0, texturedata->nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, texturedata->pixels);
                    glGenerateMipmap(GL_TEXTURE_2D);
                    Info("Texture loaded successfully: " + texturedata->t_path + " (" + to_string(texturedata->width) + "x" + to_string(texturedata->height) + ")\n");
                    stbi_image_free(texturedata->pixels);
                }
                else {
                    Logger::ERROR("Texture loaded unsuccessfully: " + texturedata->t_path + " (" + to_string(texturedata->width) + "x" + to_string(texturedata->height) + ")\n" + stbi_failure_reason() + "\n");
                    stbi_image_free(texturedata->pixels);
                }
            }
            void Load2DTexture(DATA::Texture2DData* texturedata) override {
                auto it = texturedata->Texturesid.find(texturedata->t_path);

                if (it != texturedata->Texturesid.end()) {
                    texturedata->t_id = it->second;
                    return;
                }
                createtexture(texturedata);
                loadtexture(texturedata);
                texturedata->Texturesid.emplace(texturedata->t_path, texturedata->t_id);
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

            void set_vec4(unsigned int shaderID, glm::vec4 setvec4, const GLchar* name)override {
                int loc = glGetUniformLocation(shaderID, name);
                glUniform4fv(loc, 1, glm::value_ptr(setvec4));
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

            //bool isInitialized() const override;
            openglAPI() {
                initialize();
            }
        private:

            static std::shared_ptr<openglAPI> graphic;

            bool m_initialized = false;
            bool usingdebugoutput = true;
        };
        std::shared_ptr<openglAPI> openglAPI::graphic;
    }
}