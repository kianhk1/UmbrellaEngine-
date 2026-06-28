#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <queue>
#include <mutex>

#include "../Data/Data.h"

#define øFatal(category, message) Engine::CORE::Logger::FATAL(__LINE__, std::string(__FILE__), category, message)
#define Error(category, message) Engine::CORE::Logger::ERROR(__LINE__, std::string(__FILE__), category, message)
#define Warn(category, message) Engine::CORE::Logger::WARN(__LINE__, std::string(__FILE__), category, message)
#define Info(category, message) Engine::CORE::Logger::INFO(__LINE__, std::string(__FILE__), category, message)

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
            static void log(LogLevel level, uint32_t& line, string& file, LogCategory& category, string& message);
            static void INFO(uint32_t line, string file, LogCategory category, string message);
            static void WARN(uint32_t line, string file, LogCategory category, string message);
            static void ERROR(uint32_t line, string file, LogCategory category, string message);
            static void FATAL(uint32_t line, string file, LogCategory category, string message);
            static void Flush();
        private:
            static string levelToString(LogLevel level);
            static string currentTime();
            static string ShortFile(const std::string& path);

            static std::mutex LogMutex;
            static std::queue<DATA::LogMessageData> LogQueue;
        };
    }
}

