#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <queue>
#include <mutex>
#include <sstream>

#include "../Data/Data.h"

#define øFatal(category, ...) Engine::CORE::Logger::FATAL(__LINE__, std::string(__FILE__), category, __VA_ARGS__)
#define Error(category, ...) Engine::CORE::Logger::ERROR(__LINE__, std::string(__FILE__), category, __VA_ARGS__)
#define Warn(category, ...) Engine::CORE::Logger::WARN(__LINE__, std::string(__FILE__), category, __VA_ARGS__)
#define Info(category, ...) Engine::CORE::Logger::INFO(__LINE__, std::string(__FILE__), category, __VA_ARGS__)

using namespace std;
namespace Engine{
    namespace CORE {
        enum class LogLevel {
            DBUG = 0,
            INFO,
            WARN,
            ERROR,
            FATAL
        };
        enum class LogCategory{
            Nune = 0,
            Core,
            Renderer,
            Window,
            Input,
            Resource,
            ECS,
            API,
            Audio, 
            Physics 
        };
        class Logger {
        public:
            static void log(LogLevel level, uint32_t& line, string& file, LogCategory& category, string message);
            template<typename... Args>
            static void INFO(uint32_t line, string file, LogCategory category, Args&&... args) {
                log(LogLevel::INFO, line, file, category, ToString(std::forward<Args>(args)...));
            }
            template<typename... Args>
            static void WARN(uint32_t line, string file, LogCategory category, Args&&... args) {
                log(LogLevel::WARN, line, file, category, ToString(std::forward<Args>(args)...));
            }
            template<typename... Args>
            static void ERROR(uint32_t line, string file, LogCategory category, Args&&... args) {
                log(LogLevel::ERROR, line, file, category, ToString(std::forward<Args>(args)...));
            }
            template<typename... Args>
            static void FATAL(uint32_t line, string file, LogCategory category, Args&&... args) {
                log(LogLevel::FATAL, line, file, category, ToString(std::forward<Args>(args)...));
            }
            static void Flush();
        private:
            static string levelToString(LogLevel level);
            static string currentTime();
            static string ShortFile(const std::string& path);
            template<typename... Args>
            static std::string ToString(Args&&... args)
            {
                std::ostringstream oss;
                (oss << ... << std::forward<Args>(args)); // Fold Expression
                return oss.str();
            }
            static std::vector<DATA::LogMessageData> oldLogs;
            static std::mutex LogMutex;
            static std::queue<DATA::LogMessageData> LogQueue;
        };
    }
}

