#pragma once
#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>
#include <sstream>

/*std::filesystem::exists()
std::filesystem::remove()
std::filesystem::copy()
std::filesystem::rename()
std::filesystem::create_directory()
std::filesystem::directory_iterator()*/
namespace Engine {
    namespace CORE {
        class FileSystem {
        public:
            static bool Create(const std::filesystem::path& path) {
                std::ofstream file(path);
                return file.is_open();
            }
            static bool Exists(const std::filesystem::path& path) {
                return std::filesystem::exists(path);
            }
            static void Clear(const std::filesystem::path& path) {
                std::ofstream file(path);
                file.clear();
            }

            static std::string ReadTextFile(
                const std::filesystem::path& path) {
                std::ifstream file(path);
                if (!file.is_open()) {
                    std::cerr << "Failed to open file: " << path << std::endl;
                    return "";
                }

                stringstream buf;
                buf << file.rdbuf();
                string content = buf.str();
                if (content.size() >= 3 &&
                    (unsigned char)content[0] == 0xEF &&
                    (unsigned char)content[1] == 0xBB &&
                    (unsigned char)content[2] == 0xBF) {
                    content = content.substr(3);
                }
                if (content.empty()) {
                    std::cerr << "File is empty: " << path << std::endl;
                }
                return content;
            }

            static bool WriteTextFile(
                const std::filesystem::path& path,
                const std::filesystem::path& content) {
                std::ofstream file(path, std::ios::trunc);
                if (!file.is_open()) {
                    std::cerr << "Failed to open file for writing: " << path << std::endl;
                    return false;
                }
                file << content;
                if (!file)  return false;
                return true;
            }

            static bool AppendTextFile(
                const std::filesystem::path& path,
                const std::filesystem::path& content) {
                std::ofstream file(path, std::ios::app);
                if (!file.is_open()) {
                    std::cerr << "Failed to open file for appending: " << path << std::endl;
                    return false;
                }
                file << content << std::endl;
                if (!file)  return false;
                return true;
            }

            static bool Delete(
                const std::filesystem::path& path) {
                std::error_code ec;
                if (!std::filesystem::remove(path, ec)) {
                    std::cerr << "Failed to delete file: " << path << "\nReason:" << ec.message() << std::endl;
                    return false;
                }
                return true;
            }

            static bool CopyFile(
                const std::filesystem::path& First_path,
                const std::filesystem::path& Scend_path) {
                if (!Exists(First_path)) {
                    std::cerr << "Failed to open file: " << First_path << std::endl;
                    return false;
                }
                std::error_code ec;
                std::filesystem::copy(First_path, Scend_path, ec);
                if (ec) {
                    std::cerr << "Failed to copy file: " << First_path << "to" << Scend_path << "\nReason:" << ec.message() << std::endl;
                    return false;
                }
                return true;
            }
            static bool CopyFile(
                const std::filesystem::path& First_path,
                const std::filesystem::path& Scend_path,
                std::filesystem::copy_options options) {
                if (!Exists(First_path)) {
                    std::cerr << "Failed to open file: " << First_path << std::endl;
                    return false;
                }
                std::error_code ec;
                std::filesystem::copy(First_path, Scend_path, options, ec);
                if (ec) {
                    std::cerr << "Failed to copy file: " << First_path << "to" << Scend_path << "\nReson:" << ec.message() << std::endl;
                    return false;
                }
                return true;
            }


            static bool RenameFile(const std::filesystem::path& path, const std::string& newname) {
                if (!Exists(path)) {
                    std::cerr << "Failed to open file: " << path << std::endl;
                }
                std::error_code ec;
                std::filesystem::rename(path, newname, ec);
                if (ec) {
                    std::cerr << "Failed to rename file: " << path << "\nReson:" << ec.message() << std::endl;
                    return false;
                }
                return true;
            }
            static bool CreateDirectory(const std::filesystem::path& path) {
                std::error_code ec;
                std::filesystem::create_directory(path, ec);
                if (ec) {
                    std::cerr << "Failed to create directory: " << path << "\nReson:" << ec.message() << std::endl;
                    return false;
                }
                return true;
            }
            static bool DeleteDirectory(const std::filesystem::path& path) {
                std::error_code ec;
                std::uintmax_t count = std::filesystem::remove_all(path, ec);
                if (ec)
                {
                    std::cerr << "Failed to delete directory: "
                        << path
                        << "\nReason: "
                        << ec.message()
                        << '\n';

                    return false;
                }
                return true;
            }
            static bool DeleteEmptyDirectory(const std::filesystem::path& path) {
                std::error_code ec;

                if (!std::filesystem::remove(path, ec))
                    return false;

                return !ec;
            }
            static std::vector<std::string> GetFiles(
                const std::string& path,
                std::filesystem::directory_options options = std::filesystem::directory_options::skip_permission_denied) {
                std::vector<std::string> pathfiles;
                std::error_code ec;
                auto entries = std::filesystem::recursive_directory_iterator(path, options, ec);
                if (ec) {
                    std::cerr << "Failed to get files: " << path << "\nReason:" << ec.message() << std::endl;
                    return pathfiles;
                }
                for (const auto& entry : entries) {
                    if (entry.is_regular_file())
                    {
                        pathfiles.push_back(
                            entry.path().string()
                        );
                    }
                }
                return pathfiles;
            }
        private:
            FileSystem() = delete;
        };
    }
}