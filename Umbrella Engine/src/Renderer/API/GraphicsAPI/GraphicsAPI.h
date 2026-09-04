#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <vector>
#include "../../../Core/Log Managment/Logger.h"
#include "../../../Core/Data/Data.h"

#define opengl

namespace Engine {
    namespace API {
        const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

        bool initialize(); // مقداردهی اولیه API
        void shutdown();   // پاکسازی منابع

        void setViewport(int x, int y, int width, int height);
        void clearColor(float r, float g, float b, float a);
        enum Buffers
        {
            COLOR = 1 << 0,
            DEPTH = 1 << 1,
            STENCIL = 1 << 2,
            ALL = 7
        };
        void clearBuffers(int buffers); // پاک کردن بافر رنگ و عمق

        // مدیریت شیدرها
        unsigned int createShader();
        unsigned int compileShader(unsigned int type, const std::string source);
        bool linkShaderProgram(GLuint programID, GLuint vertexShaderID, GLuint fragmentShaderID);
        void deleteShader(DATA::ShaderData& shader);
        void useShader(const DATA::ShaderData& shader);

        // مدیریت مش‌ها (Vertex/Index data)
        void setAttrib(DATA::MeshData& data, int a, int b, int c, int d);
        DATA::MeshData createMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices);
        void deleteMesh(DATA::MeshData& buffers);
        void drawMesh(DATA::MeshData& buffers, DATA::RenderState state);

        unsigned int createUBO(long long int size_ptr);
        void UpdateBuffer(unsigned int UBO, void* data, size_t offset, size_t datasize);
        void BindBuffer(unsigned int UBO, int slotNumber, int offset, int size);

        unsigned int createFBO(unsigned int dataMap, int type);
        unsigned int createdepthmap();
        unsigned int createcolorhmap();
        void generamaptexture(unsigned int map, unsigned int FBO, unsigned int type);
        void BindFBO(unsigned int FBO);

        // texture
        void createtexture2d(std::shared_ptr<DATA::TextureData> texturedata);
        bool loadtexture2d(std::shared_ptr<DATA::TextureData> texturedata);

        void createcubemap(std::shared_ptr<DATA::TextureData> texturedata);
        bool loadcubemap(std::shared_ptr<DATA::TextureData> texturedata);

        void Bind(std::shared_ptr<DATA::TextureData> texturedata);
        void Bind(unsigned int unit, unsigned int textureID);

        //
        void ReflectUniforms(std::shared_ptr<DATA::ShaderData> shader);
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
            const GLchar* name);
    }
}