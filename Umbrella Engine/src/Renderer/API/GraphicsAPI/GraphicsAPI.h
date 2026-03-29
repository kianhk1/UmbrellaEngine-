#pragma once
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <vector>
#include "../../../Core/Log Managment/Logger.h"

using namespace std;

struct ShaderData {
	unsigned int programID;
};
struct Size { int width; int height; };
struct Point { int x; int y; };

class Mesh;
using FuncPtr = void(*)();
namespace API {
    class GraphicsAPI {
    public:
        virtual ~GraphicsAPI() = default;


        virtual void InitWindow(int width, int height, const char* title) = 0;
        virtual void CloseWindow(void) = 0;
        virtual bool IsWindowShouldClose(void) = 0;
        virtual bool IsWindowReady() = 0;
        virtual bool IsWindowFullscreen(void) = 0;
        virtual bool IsWindowMinimized(void) = 0;
        virtual bool IsWindowMaximized(void) = 0;
        virtual bool IsDebugOn() = 0;

        virtual void SetFullscreen(bool fullscreen) = 0;
        virtual void ToggleFullscreen() = 0;
        virtual void ToggleBorderlessWindowed(void) = 0;                       // Toggle window state: borderless windowed, resizes window to match monitor resolution
        virtual void MaximizeWindow(void) = 0;                                  // Set window state: maximized, if resizable
        virtual void MinimizeWindow(void) = 0;                                  // Set window state: minimized, if resizable
        virtual void RestoreWindow(void) = 0;                                  // Set window state: not minimized/maximized
        virtual void SetWindowIcon(string imagePath) = 0;

        virtual void ShowCursor(void) = 0;                                     /// Shows cursor
        virtual void HideCursor(void) = 0;                                    /// Hides cursor
        virtual bool IsCursorHidden(void) = 0;                                 /// Check if cursor is not visible
        virtual void EnableCursor(void) = 0;                                    /// Enables cursor (unlock cursor)
        virtual void DisableCursor(void) = 0;                                   /// Disables cursor (lock cursor)
        virtual bool IsCursorOnScreen(void) = 0;
        virtual Size GetSize() = 0;

        virtual bool initialize() = 0; // مقداردهی اولیه API
        virtual void shutdown() = 0;   // پاکسازی منابع

        virtual void setViewport(int x, int y, int width, int height) = 0;
        virtual void clearColor(float r, float g, float b, float a) = 0;
        virtual void clearBuffers() = 0; // پاک کردن بافر رنگ و عمق

        // مدیریت شیدرها
        virtual ShaderData createShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) = 0;
        virtual void deleteShader(ShaderData& shader) = 0;
        virtual void useShader(const ShaderData& shader) = 0;

        // مدیریت مش‌ها (Vertex/Index data)
        virtual Mesh* createMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices, FuncPtr Attrib) = 0;
        virtual void deleteMesh(Mesh* buffers) = 0;
        virtual void drawMesh(Mesh* buffers) = 0;

        //
        virtual void set_mat4(glm::mat4 setmat4, const GLchar* name) = 0;
        virtual void set_vec3(glm::vec3 setvec3, const GLchar* name) = 0;
        virtual void set_vec3(int x, int y, int z, const GLchar* name) = 0;
        virtual void set_float(float setfloat, const GLchar* name) = 0;
        virtual void set_int(int setint, const GLchar* name) = 0;
        virtual void set_bool(bool setbool, const GLchar* name) = 0;

        // دریافت وضعیت API (مثلاً آیا اولیه شده است)
        virtual bool isInitialized() const = 0;
    protected:
        string Read_File(const char* path) {
            ifstream file(path);
            stringstream buf;
            buf << file.rdbuf();
            string content = buf.str();
            if (content.size() >= 3 &&
                (unsigned char)content[0] == 0xEF &&
                (unsigned char)content[1] == 0xBB &&
                (unsigned char)content[2] == 0xBF) {
                content = content.substr(3);
            }
            return content;
        }
    };
}

