#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;
enum class LogLevel{
	INFO,
	WARN,
	ERROR,
	FATAL
};
enum class InitError{
    error,
    GLFW_INIT_FAILED_Error,
    WINDOW_CREATE_FAIL_Error,
    GLAD_INIT_FAILED_Error,
    Shader_Compile_Error,
    Shader_Create_Error,
    unused_uniform_Error,
    uniform_not_found_Warning,
    Texture_not_bound_Warning,
    GLFW_INIT,
    WINDOW_CREATE,
    GLAD_INIT,
    Shader_Compile,
    Shader_Create,
    Set_uniform,
    Texture_bound
};
class Logger {
public:
    static void error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
    static void Get_Error(InitError stat, int id, string where, const char* FILE);
    static void log(LogLevel level, string masseage);
    static void INFO(string masseage);
    static void WARN(string masseage);
    static void ERROR(string masseage);
    static void FATAL(string masseage);
    static string get_msg() {
        return messages;
    }

private:
    static string levelToString(LogLevel level);
    static string currentTime();
    static string messages;
};

