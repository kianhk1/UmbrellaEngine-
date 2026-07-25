#include <iostream>
#include <glad/glad.h>
#include <memory>
#include "GraphicsAPI.h"
#include "../../../Input & Output Manager/Input & Output File/ReadFile.h"
#include <any>

namespace Engine {
    namespace API {
#ifdef opengl
        bool m_initialized = false;
        bool usingdebugoutput = true;
        void InitDebugOutPut(GLDEBUGPROC callback) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(callback, nullptr);
        }
        bool IsDebugOn() { return usingdebugoutput; }
        void DebugTurnOn() { usingdebugoutput = true; }
        void DebugTurnOff() { usingdebugoutput = false; }

        // مقداردهی اولیه API
        bool initialize() {
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
                return false;
            const GLubyte* version = glGetString(GL_VERSION);
            Info(Engine::CORE::LogCategory::API,
                "OpenGL Version: " , reinterpret_cast<const char*>(version));
            if (usingdebugoutput) {
                //InitDebugOutPut(Logger::error_callback);
            }
            glEnable(GL_DEPTH_TEST);
            m_initialized = true;
            return true;
        }

        // پاکسازی منابع
        void shutdown() {
            if (!m_initialized) return;
            // اینجا می‌توان منابع گلوبال OpenGL را پاک کرد، اگر وجود داشته باشد
            Info(Engine::CORE::LogCategory::API, "OpenGL Graphics API Shutting Down.");
            m_initialized = false;
        }

        void setViewport(int x, int y, int width, int height) {
            glViewport(x, y, width, height);
        }
        void clearColor(float r, float g, float b, float a) {
            glClearColor(r, g, b, a);
        }

        // پاک کردن بافر رنگ و عمق
        void clearBuffers() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        }

        // مدیریت شیدرها
        unsigned int createShader() {
            unsigned int programID;
            return programID = glCreateProgram();
        }
        unsigned int compileShader(unsigned int type, const std::string source) {

            unsigned int shaderID = glCreateShader(type);
            const char* src = source.c_str();
            glShaderSource(shaderID, 1, &src, nullptr);
            glCompileShader(shaderID);
            int success;
            char infoLog[512];
            glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
                Error(Engine::CORE::LogCategory::API, "Shader compilation failed: " , infoLog);
                glDeleteProgram(shaderID);
                return 0;
            }
            return shaderID;
        }
        bool linkShaderProgram(GLuint programID, GLuint vertexShaderID, GLuint fragmentShaderID) {
            glAttachShader(programID, vertexShaderID);
            glAttachShader(programID, fragmentShaderID);
            glLinkProgram(programID);

            int success;
            char infoLog[512];
            glGetProgramiv(programID, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(programID, 512, nullptr, infoLog);
                Error(Engine::CORE::LogCategory::API, "Shader program linking failed: " , infoLog);
                glDeleteProgram(programID);
                return false;
            }
            glDeleteShader(vertexShaderID);
            glDeleteShader(fragmentShaderID);
            return true;
        }
        void deleteShader(DATA::ShaderData& shader) {
            if (shader.programID != 0) {
                glDeleteProgram(shader.programID);
                shader.programID = 0;
            }
        }
        void useShader(const DATA::ShaderData& shader) {
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
        void setAttrib(DATA::MeshData& data, int a, int b, int c, int d) {
            glBindVertexArray(data.vaoID);
            glEnableVertexAttribArray(a);
            glBindBuffer(GL_ARRAY_BUFFER, data.vboID);
            glVertexAttribPointer(a, b, GL_FLOAT, GL_FALSE, c * sizeof(float), (void*)(d * sizeof(float)));
        }
        DATA::MeshData createMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices) {
            DATA::MeshData buffers;
            createVAO(buffers);
            createVBO(buffers, vertices.data(), vertices.size());
            createIBO(buffers, indices.data(), indices.size());
            buffers.indexCount = indices.size();
            return buffers;
        }
        void deleteMesh(DATA::MeshData& buffers) {
            if (buffers.vaoID != 0) glDeleteVertexArrays(1, &buffers.vaoID);
            if (buffers.vboID != 0) glDeleteBuffers(1, &buffers.vboID);
            if (buffers.iboID != 0) glDeleteBuffers(1, &buffers.iboID);
            buffers.vaoID = buffers.vboID = buffers.iboID = 0;
            buffers.indexCount = 0;
        }
        void drawMesh(DATA::MeshData& buffers, DATA::RenderState state) {
            glDepthMask(state.depthtest);
            glDepthFunc(GL_LESS);
            glBindVertexArray(buffers.vaoID);
            glDrawElements(GL_TRIANGLES, buffers.indexCount, GL_UNSIGNED_INT, 0);
        }
        unsigned int createUBO(long long int size_ptr) {
            unsigned int UBO;
            glGenBuffers(1, &UBO);
            glBindBuffer(GL_UNIFORM_BUFFER, UBO);
            glBufferData(GL_UNIFORM_BUFFER, size_ptr, NULL, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            return UBO;
        }
        void BindBuffer(unsigned int UBO, int slotNumber, int offset, int size) {
            glBindBuffer(GL_UNIFORM_BUFFER, UBO);
            glBindBufferRange(GL_UNIFORM_BUFFER, slotNumber, UBO, offset, size);
            //glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }
        void UpdateBuffer(unsigned int UBO, void* data, size_t offset, size_t datasize) {
            glBindBuffer(GL_UNIFORM_BUFFER, UBO);
            glBufferSubData(GL_UNIFORM_BUFFER, offset, datasize, data);
            //glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

#define ActiveBuffer(Buffer, code) glBindBuffer(GL_UNIFORM_BUFFER, Buffer); code glBindBuffer(GL_UNIFORM_BUFFER, 0)

        // texture ...........
        void createtexture(std::shared_ptr<DATA::TextureData> texturedata) {
            glGenTextures(1, &texturedata->id);
            glBindTexture(GL_TEXTURE_2D, texturedata->id);
        }
        bool loadtexture2d(std::shared_ptr<DATA::TextureData> texturedata) {
            createtexture(texturedata);
            // set the texture wrapping/filtering options (on currently bound texture)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // load and generate the texture
            CORE::ImageData images = CORE::Reader::Readimage(texturedata->paths[0]);
            if (images.log.empty()) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images.width, images.height, 0, texturedata->nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, images.pixels);
                glGenerateMipmap(GL_TEXTURE_2D);
                Info(Engine::CORE::LogCategory::API, "Texture loaded successfully: " ,
                    texturedata->paths[0] , " (" , images.width , "x" , images.height , ")");
                CORE::Reader::Freeimage(images);
                return true;
            }
            else {
                Error(Engine::CORE::LogCategory::API, "Texture loaded unsuccessfully: " ,
                    texturedata->paths[0] , " (" , images.width , "x" ,
                    images.height , ")\n" , images.log , "\n");
                CORE::Reader::Freeimage(images);
                return false;
            }
        }
        void Bind(std::shared_ptr<DATA::TextureData> texturedata) {
            glActiveTexture(texturedata->unit + GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texturedata->id);
        }

        //
        void SetUniform(
            unsigned int shaderID, 
            std::variant<
            int,
            float,
            glm::vec2,
            glm::vec3, 
            glm::vec4,
            glm::mat4
            > value, 
            const GLchar* name) {
            int loc = glGetUniformLocation(shaderID, name);
            if (loc == -1) {
                Warn(Engine::CORE::LogCategory::API, "Uniform '" , name , "' not found in shader program " , shaderID);
                return;
            }
            std::visit([&](auto&& value)
                {
                    using T = std::decay_t<decltype(value)>; 

                    if constexpr (std::is_same_v<T, int>) 
                    {
                        glUniform1i(loc, value); 
                    }
                    else if constexpr (std::is_same_v<T, float>)
                    {
                        glUniform1f(loc, value);
                    }
                    else if constexpr (std::is_same_v<T, glm::vec2>)
                    {
                        glUniform2fv(loc, 1, glm::value_ptr(value));
                    }
                    else if constexpr (std::is_same_v<T, glm::vec3>)
                    {
                        glUniform3fv(loc, 1, glm::value_ptr(value));
                    }
                    else if constexpr (std::is_same_v<T, glm::vec4>)
                    {
                        glUniform4fv(loc, 1, glm::value_ptr(value));
                    }
                    else if constexpr (std::is_same_v<T, glm::mat4>)
                    {
                        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
                    }
                    else {
                        Warn(Engine::CORE::LogCategory::API, "Uniform type not soported in shader program.");
                    }
                }, value);
            
            return;
        }
        //bool isInitialized() const ;
#endif
    }
}