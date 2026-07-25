#include "Logger.h"

namespace Engine {
    namespace CORE {
        void error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
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
                //FATAL(msg);
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                //ERROR(msg);
                break;
            case GL_DEBUG_SEVERITY_LOW:
                //WARN(msg);
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                //INFO(msg);
                break;
            }
        }


        void Logger::log(LogLevel level, uint32_t& line, string& file, LogCategory& category, string message) {
            DATA::LogMessageData Log;
            Log.level = level; 
            Log.time = currentTime();
            Log.line = line;
			Log.file = ShortFile(file);
            Log.category = category; 
            Log.message = message; 
            std::lock_guard<std::mutex> lock(LogMutex);
            auto currentlog = std::find(oldLogs.begin(), oldLogs.end(), Log);

            if (currentlog == oldLogs.end())
            {
                if (level == LogLevel::FATAL)
                    Log.message += "\nFatal error encountered. Exiting program.";

                LogQueue.push(Log);
                oldLogs.push_back(Log);
            }
            else if (currentlog->message != Log.message)
            {
                Log.hederprint = false;
                LogQueue.push(Log);
                oldLogs.push_back(Log);
            }
        }


        void Logger::Flush() {
            std::lock_guard<std::mutex> lock(LogMutex);
            while (!LogQueue.empty()) {
                const auto& log = LogQueue.front();
                auto category = [](LogCategory category) {
                    switch (category)
                    {
                    case Engine::CORE::LogCategory::Nune:
                        return "Nune";
                        break;
                    case Engine::CORE::LogCategory::Core:
                        return "Core";
                        break;
                    case Engine::CORE::LogCategory::Renderer:
                        return "Renderer";
                        break;
                    case Engine::CORE::LogCategory::Window:
                        return "Window";
                        break;
                    case Engine::CORE::LogCategory::Input:
                        return "Input";
                        break;
                    case Engine::CORE::LogCategory::Resource:
                        return "Resource";
                        break;
                    case Engine::CORE::LogCategory::ECS:
                        return "ECS";
                        break;
                    case Engine::CORE::LogCategory::API:
                        return "API";
                        break;
                    case Engine::CORE::LogCategory::Audio:
                        return "Audio";
                        break;
                    case Engine::CORE::LogCategory::Physics:
                        return "Physics";
                        break;
                    default:
                        break;
                    }
                    };
                if (log.hederprint)
                    std::cout << "[" << levelToString(log.level) << "][" << log.time
                        << "][\033[1;34m" << log.file << ":" << log.line << "\033[0m]"
                        << "[" << category(log.category) << "]\n" << log.message << std::endl;
                else
                    std::cout << log.message << std::endl;

                LogQueue.pop();
            }
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
        string Logger::ShortFile(const std::string& path) {
            size_t pos = path.find("src");

            if (pos != std::string::npos)
                return path.substr(pos);

            return path;
        }
        std::vector<DATA::LogMessageData> Logger::oldLogs;
        std::mutex Logger::LogMutex;
        std::queue<DATA::LogMessageData> Logger::LogQueue;
    }
}