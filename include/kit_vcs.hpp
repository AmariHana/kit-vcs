#ifndef KIT_VCS_HPP
#define KIT_VCS_HPP

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <sstream>
#include "kit_utils.hpp"
#include "hash_object.hpp" // Ensure this header defines hash_object and compute_sha1

namespace kit_vcs
{

    // Constants
    constexpr auto KIT_DIR = ".kit";
    constexpr auto OBJECTS_DIR = ".kit/objects";
    constexpr auto REFS_DIR = ".kit/refs";
    constexpr auto HEADS_DIR = ".kit/refs/heads";
    constexpr auto HEAD_FILE = ".kit/HEAD";
    constexpr auto INDEX_FILE = ".kit/index";

    // Repository Initialization
    inline bool is_repository_initialized()
    {
        return std::filesystem::exists(KIT_DIR);
    }

    inline void create_file(const std::string &file_path, const std::string &content = "")
    {
        std::ofstream file(file_path);
        if (!file)
        {
            throw std::ios_base::failure("Failed to create file: " + file_path);
        }
        file << content;
        file.close();
    }

    inline void initialize_repository()
    {
        if (is_repository_initialized())
        {
            kit_utils::print_error("A kit repository already exists in this directory.");
            return;
        }

        try
        {
            std::filesystem::create_directories(OBJECTS_DIR);
            std::filesystem::create_directories(HEADS_DIR);
            create_file(HEAD_FILE, "ref: refs/heads/main\n");
            create_file(INDEX_FILE);
            kit_utils::print_message("Kit repository initialized successfully.");
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to initialize repository: " + std::string(e.what()));
        }
    }

    // Check if a file is already staged
    inline bool is_file_staged(const std::string &file_path)
    {
        std::ifstream index_file(INDEX_FILE);
        if (!index_file)
        {
            return false; // If the index file doesn't exist, the file is not staged
        }

        std::string line;
        while (std::getline(index_file, line))
        {
            if (line.find(file_path) != std::string::npos)
            {
                return true; // File is already staged
            }
        }
        return false;
    }

    // Stage a file
    inline bool stage_file(const std::string &file_path)
    {
        if (is_file_staged(file_path))
        {
            kit_utils::print_message("File is already staged: " + file_path);
            return false; // Prevent duplicate staging
        }

        try
        {
            // Read the file content
            std::ifstream file(file_path, std::ios::binary);
            if (!file)
            {
                kit_utils::print_error("Failed to read file: " + file_path);
                return false;
            }
            std::ostringstream buffer;
            buffer << file.rdbuf();
            std::string content = buffer.str();

            // Compute the hash of the file content
            std::string hash = hash_object::compute_sha1(content);

            // Add the file to the index
            std::ofstream index_file(INDEX_FILE, std::ios::app);
            if (!index_file)
            {
                kit_utils::print_error("Failed to open index file.");
                return false;
            }
            index_file << hash << " " << file_path << "\n";
            index_file.close();

            kit_utils::print_message("File staged successfully: " + file_path);
            return true;
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to stage file: " + std::string(e.what()));
            return false;
        }
    }

    // Repository Operations
    inline bool has_staged_files()
    {
        std::ifstream index_file(INDEX_FILE);
        if (!index_file)
        {
            kit_utils::print_error("Failed to open index file.");
            return false;
        }
        std::string line;
        while (std::getline(index_file, line))
        {
            if (!line.empty())
                return true;
        }
        return false;
    }

    inline bool create_commit(const std::string &message)
    {
        if (!has_staged_files())
        {
            kit_utils::print_error("No files staged for commit.");
            return false;
        }
        kit_utils::print_message("Commit created successfully with message: " + message);
        return true;
    }

    inline std::vector<std::string> get_repository_status()
    {
        std::vector<std::string> status;

        if (!is_repository_initialized())
        {
            kit_utils::print_error("No kit repository found in this directory.");
            return status;
        }

        try
        {
            if (has_staged_files())
            {
                status.push_back("Staged files are ready for commit.");
            }
            else
            {
                status.push_back("No files staged for commit.");
            }

            for (const auto &entry : std::filesystem::recursive_directory_iterator("."))
            {
                if (entry.is_regular_file() && entry.path().string().find(KIT_DIR) == std::string::npos)
                {
                    status.push_back("Untracked file: " + entry.path().string());
                }
            }
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to retrieve repository status: " + std::string(e.what()));
        }

        return status;
    }

} // namespace kit_vcs

#endif // KIT_VCS_HPP
