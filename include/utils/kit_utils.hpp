#ifndef KIT_UTILS_HPP
#define KIT_UTILS_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace kit_utils
{
    inline void print_message(const std::string &message)
    {
        std::cout << "[kit] " << message << std::endl;
    }

    inline void print_error(const std::string &message)
    {
        std::cerr << "[kit] Error: " << message << std::endl;
    }

    inline bool ensure_repository_initialized()
    {
        if (!std::filesystem::exists(".kit"))
        {
            print_error("No kit repository found. Please initialize a repository first.");
            return false;
        }
        return true;
    }

    inline void create_file(const std::string &path, const std::string &content = "")
    {
        std::ofstream file(path);
        if (!file)
        {
            throw std::runtime_error("Failed to create file: " + path);
        }
        file << content;
        file.close();
    }

    inline bool has_staged_files()
    {
        return std::filesystem::exists(INDEX_FILE) && std::filesystem::file_size(INDEX_FILE) > 0;
    }
} // namespace kit_utils

#endif // KIT_UTILS_HPP
