#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <vector>
#include <variant>
#include <unordered_map>
#include <entt/entt.hpp>
#include "../../Editor/ImGui/imgui.h"
/*#define GL_NEVER 0x0200
#define GL_LESS 0x0201
#define GL_EQUAL 0x0202
#define GL_LEQUAL 0x0203
#define GL_GREATER 0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL 0x0206
#define GL_ALWAYS 0x0207*/
namespace Engine {
    namespace CORE {
        enum class LogLevel;
        enum class LogCategory;
    }
    namespace API {
        enum KeyboardKey;
		enum MouseButton;
    }
    enum class TextureType;
    namespace DATA {


        struct TextureHandle
        {
            TextureHandle() = default;
            TextureHandle(uint64_t id) : ID(id) {}
            uint64_t ID = 0;
        };
        struct ShaderHandle
        {
            ShaderHandle() = default;
            ShaderHandle(uint64_t id) : ID(id) {}
            uint64_t ID = 0;
        };
        struct ModelHandle
        {
            ModelHandle() = default;
            ModelHandle(uint64_t id) : ID(id) {}
            uint64_t ID = 0;
            
        };
        struct ScriptHandle
        {
            ScriptHandle() = default;
            ScriptHandle(uint64_t id) : ID(id) {} 
            uint64_t ID = 0; 

        };

        struct LightData {
            glm::vec3 lightcolor = glm::vec3(1.0f);
        };
        
        struct CameraData {
            glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::mat4 projection;
            glm::mat4 view;

            float fov = 90.0f;
            float windowWidth;
            float windowHeight;
            float AspectRatio = windowWidth / windowHeight;
            float NCP = 0.1f;
            float FCP = 100;
        };

        struct RenderState
        {
            bool depthtest = true;
            bool shadow = true;
            bool isTransparent = false;
            int depthfunc = 0x0201;
        };
        using Uniform = std::variant<
            int,
            float,
            glm::vec2,
            glm::vec3,
            glm::vec4,
            glm::mat4
        >;
        enum class UniformType
        {
            INT,
            FLOAT,
            VEC2,
            VEC3,
            VEC4,
            MAT4
        };
        struct ShaderData {
            std::string path_Vertex_Shader;
            std::string path_Fragment_Shader;
            unsigned int programID = -1;
            std::unordered_map<std::string, UniformType> uniforms;
        };

        struct MeshData {
            MeshData() = default;
            unsigned int vaoID = 0; 
            unsigned int vboID = 0;
            unsigned int iboID = 0;
            int indexCount = 0;
            std::vector<float> vertices;
            std::vector<unsigned int> indices;
        };

        struct TextureDesc {
            std::vector<std::string> paths;
            bool isLinear = true;
        };
        struct TextureData {
            std::vector<std::string> paths;
            TextureType type;
            uint32_t  id;
            uint32_t Width;
            uint32_t Height;
            int unit;
            int nrChannels;
            bool isLinear = true;
        };


        struct MaterialData {
            std::unordered_map<
                std::string,
                TextureHandle
            > textures;
            std::unordered_map<std::string, Uniform> uniforms;
            
        };
       
        struct Node
        {
            int cc = 0;
            std::vector<std::shared_ptr<Node>> children;
            std::string name;
            glm::mat4 localTransform;
            glm::mat4 worldTransform;
            std::vector<uint32_t> meshIndices;

            template<typename F>
            void each(F&& f, int depth = 0) 
            {
                f(*this, depth);
                
                for (auto& child : children)
                {

                    child->each(std::forward<F>(f), depth + 1);
                }
            }
        };
        struct ModelPart
        {
            MeshData mesh;
            MaterialData material;
        };
        struct ModelData
        {
            std::string path;
            std::shared_ptr<DATA::Node> root;
            std::vector<ModelPart> parts; 
            ModelPart& GetMaterial(uint32_t index)
            {
                return parts[index];
            }

            const ModelPart& GetMaterial(uint32_t index) const
            {
                return parts[index];
            }
        };

        struct Size { int width; int height; };
        struct Point { int x; int y; };

        struct KeyData {
            int action;
            int mod = 0;
        };
        struct ButtonData {
            int mod = 0;
            int action;
        };

        struct WindowContext;
        struct windowData {
            std::shared_ptr<WindowContext> windowcontext;
            Point position;                     // Window position (required on fullscreen toggle)
            Size display;                       // Display width and height (monitor, device-screen, LCD, ...)
            Size screen;                        // Screen width and height (used render area)
            Size screenMin;                     // Screen minimum width and height (for resizable window)
            Size screenMax;                     // Screen maximum width and height (for resizable window)

            const char* title;                  // Window text title const pointer
            unsigned int flags;                 // Configuration flags (bit based), keeps window state

            bool ready;                         // Check if window has been initialized successfully
            bool shouldClose = false;                   // Check if window set for closing
            bool hidecursor;
            bool m_isMinimized;
            bool m_isMaximized;
            bool m_isFullscreen;
			//ButtonData button;
			//KeyData key;

            KeyData keyboardKey[512];                  // Keyboard key pressed
			ButtonData mouseButton[8];                    // Mouse button pressed

			Point mousepos;                        // Mouse position
        };

        struct LogMessageData
        {
            CORE::LogLevel level;
            CORE::LogCategory category;
            std::string message;
            std::string file;
            uint32_t line;
            std::string time;
            bool hederprint = true;
            bool operator==(const LogMessageData& other) const
            {
                return level == other.level &&
                    category == other.category &&
                    file == other.file &&
                    line == other.line;
            }

        };
        struct ConsoleSegment
        {
            std::string text;
            ImVec4 color;
        };

        struct ConsoleLine
        {
            std::vector<ConsoleSegment> segments;
        };

        struct Payload {
            void* Sender;
        };
        struct ShaderDesc
        {
            std::string path_Vertex_Shader, path_Fragment_Shader;
        };
    }
}
