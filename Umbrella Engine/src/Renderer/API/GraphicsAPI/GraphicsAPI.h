#pragma once
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <vector>

#include "../../../Core/Log Managment/Logger.h"
#include "../../Data/Data.h"

using namespace std;

namespace Engine {
    namespace API {
        class GraphicsAPI {
        public:
            
            virtual ~GraphicsAPI() = default;


            virtual bool initialize() = 0; // مقداردهی اولیه API
            virtual void shutdown() = 0;   // پاکسازی منابع

            virtual void setViewport(int x, int y, int width, int height) = 0;
            virtual void clearColor(float r, float g, float b, float a) = 0;
            virtual void clearBuffers() = 0; // پاک کردن بافر رنگ و عمق

            // مدیریت شیدرها
            virtual unsigned int createShader() = 0;
            virtual unsigned int compileShader(unsigned int type, const std::string source) = 0;
            virtual bool linkShaderProgram(GLuint programID, GLuint vertexShaderID, GLuint fragmentShaderID) = 0;
            virtual void deleteShader(DATA::ShaderData& shader) = 0;
            virtual void useShader(const DATA::ShaderData& shader) = 0;

            // مدیریت مش‌ها (Vertex/Index data)
            virtual void setAttrib(DATA::MeshData& data, int a, int b, int c, int d) = 0;
            virtual DATA::MeshData createMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices) = 0;
            virtual void deleteMesh(DATA::MeshData& buffers) = 0;
            virtual void drawMesh(DATA::MeshData& buffers) = 0;
            virtual unsigned int createUBO(long long int size_ptr) = 0;
            virtual void UpdateBuffer(unsigned int UBO, void* data, size_t offset, size_t datasize) = 0;
            virtual void BindBuffer(unsigned int UBO, int slotNumber, int offset, int size) = 0;

            // texture
            virtual void Load2DTexture(DATA::Texture2DData* texturedata) = 0;
            virtual void Bind(DATA::Texture2DData* texturedata) = 0;

            //
            virtual void set_mat4(unsigned int shaderID, glm::mat4 setmat4, const GLchar* name) = 0;
            virtual void set_vec3(unsigned int shaderID, glm::vec3 setvec3, const GLchar* name) = 0;
            virtual void set_vec4(unsigned int shaderID, glm::vec4 setvec4, const GLchar* name) = 0;
            virtual void set_float(unsigned int shaderID, float setfloat, const GLchar* name) = 0;
            virtual void set_int(unsigned int shaderID, int setint, const GLchar* name) = 0;
            virtual void set_bool(unsigned int shaderID, bool setbool, const GLchar* name) = 0;
        };
    }
}