#include "Logger.h"

//#define warn(x) Logger::WARN("[" +to_string(__LINE__)+ "]" + x)
std::string Logger::messages;

void Logger::error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    string msg;
    switch (source) {
    case GL_DEBUG_SOURCE_API:
        msg += string("[") + "\033[1;34mSOURCE:API" + "\033[0m]";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        msg += string("[") + "\033[1;34mSOURCE:WINDOW SYSTEM" + "\033[0m]";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        msg += string("[") + "\033[1;34mSOURCE:SHADER COMPILER" + "\033[0m]";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        msg += string("[") + "\033[1;34mSOURCE:THIRD PARTY" + "\033[0m]";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        msg += string("[") + "\033[1;34mSOURCE:APPLICATION" + "\033[0m]";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        msg += string("[") + "\033[1;34mSOURCE:OTHER" + "\033[0m]";
        break;
    }
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        msg += string("[") + "\033[1;34mTYPE:ERROR" + "\033[0m]";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        msg += string("[") + "\033[1;34mTYPE:DEPRECATED BEHAVIOR" + "\033[0m]";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        msg += string("[") + "\033[1;34mTYPE:UNDEFINED BEHAVIOR" + "\033[0m]";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        msg += string("[") + "\033[1;34mTYPE:PORTABILITY" + "\033[0m]";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        msg += string("[") + "\033[1;34mTYPE:PERFORMANCE" + "\033[0m]";
        break;
    case GL_DEBUG_TYPE_OTHER:
        msg += string("[") + "\033[1;34mTYPE:OTHER" + "\033[0m]";
        break;
    }
    msg += message;
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        FATAL(msg);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        ERROR(msg);
        break;
    case GL_DEBUG_SEVERITY_LOW:
        WARN(msg);
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        Info(msg);
        break;
    }
}
void Logger::Get_Error(InitError stat, int id, string where, const char* FILE) {
    char LOG[1024];
    int type;
    string TYPE;
    switch (stat) {
    case InitError::GLFW_INIT_FAILED_Error:
        Logger::FATAL(string("[Failed to initialize GLFW]\n") + "[\033[1;34mFILE:" + FILE + "\033[0m]\n");
        break;
    case InitError::WINDOW_CREATE_FAIL_Error:
        Logger::FATAL(string("[Failed to create window]\n") + "[\033[1;34mFILE:" + FILE + "\033[0m]\n");
        break;
    case InitError::GLAD_INIT_FAILED_Error:
        Logger::FATAL(string("[Failed to initialize GLAD]\n") + "[\033[1;34mFILE:" + FILE + "\033[0m]\n");
        break;
    case InitError::Shader_Compile_Error:
        glGetShaderInfoLog(id, 1024, nullptr, LOG);
        glGetShaderiv(id, GL_SHADER_TYPE, &type);
        if (type == 0x8B31) TYPE = "VERTEX_SHADER";
        else if (type == 0x8B30) TYPE = "FRAGMENT_SHADER";
        Logger::ERROR(std::string("[Shader Compile Error]") + "[\033[1;34mTYPE=" + TYPE + "\033[0m]" + "[\033[1;34mFILE=" + where + "\033[0m]" + "[\033[1;34mLINE:" + FILE + "\033[0m]\n" + "[\033[1;34mID=" + to_string(id) + "\033[0m]\n" + "Details:\n" + string(LOG));
        break;
    case InitError::Shader_Create_Error:
        Logger::ERROR(string("[Shader create Error]") + "[\033[1;34mFILE:" + FILE + "\033[0m]\n" + "[\033[1;34mprogram=" + to_string(id) + "\033[0m]\n" + "Details:\n" + messages + '\n');
        break;
    case InitError::unused_uniform_Error:
        if (type == 0x8B31) TYPE = "VERTEX_SHADER";
        else if (type == 0x8B30) TYPE = "FRAGMENT_SHADER";
        Logger::ERROR(std::string("[Shader Compile Error]") + "[\033[1;34mTYPE=" + TYPE + "\033[0m]" + "[\033[1;34mFILE=" + where + "\033[0m]" + "[\033[1;34mID=" + to_string(id) + "\033[0m]\n" + "Details:\n" + string(LOG));
        break;
    case InitError::error:
        Logger::ERROR("[\033[1;34mLINE:" + where + "\033[0m]" + "[\033[1;34mFILE:" + FILE + "\033[0m]\n" + "Details:\n" + messages + '\n');
        break;
    }
}
void Logger::log(LogLevel level, string masseage) {
    string log_level = levelToString(level);
    string timest = currentTime();
    cout << "[" << log_level << "][" << timest << "]" << masseage;

    if (level == LogLevel::FATAL) {
        std::cerr << masseage << "\nFatal error encountered. Exiting program.\n";
        exit(EXIT_FAILURE);
    }
}
void Logger::INFO(string masseage) {
    log(LogLevel::INFO, masseage);
}
void Logger::WARN(string masseage) {
    log(LogLevel::WARN, masseage);
}
void Logger::ERROR(string masseage) {
    log(LogLevel::ERROR, masseage);
}
void Logger::FATAL(string masseage) {
    log(LogLevel::FATAL, masseage);
}


string Logger::levelToString(LogLevel level) {
    switch (level) {
    case LogLevel::INFO: return "\033[1;32mINFO\033[0m";
    case LogLevel::WARN: return "\033[1;33mWARN\033[0m";
    case LogLevel::ERROR: return "\033[1;31mERROR\033[0m";
    case LogLevel::FATAL: return "\033[1;35mFATAL\033[0m";
    }
}
string Logger::currentTime() {
    std::time_t now = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &now);
    std::ostringstream oss;
    oss << 1900 + localTime.tm_year << "/"
        << 1 + localTime.tm_mon << "/"
        << localTime.tm_mday << "-"
        << localTime.tm_hour << ":"
        << localTime.tm_min << ":"
        << localTime.tm_sec;
    return oss.str();
}