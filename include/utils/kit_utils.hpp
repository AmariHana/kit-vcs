#ifndef KIT_UTILS_HPP
#define KIT_UTILS_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <ctime>
#include <stdexcept>
#include "constants.hpp"
#include "error_handler.hpp"
#include "hash_object.hpp"

namespace kit_utils
{
    // Read the content of a file
    inline std::string read_file(const std::string &path)
    {
        try
        {
            std::ifstream file(path);
            if (!file)
            {
                throw std::runtime_error("Failed to open file: " + path);
            }
            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }
        catch (const std::exception &e)
        {
            error_handler::print_error("Error reading file: " + std::string(e.what())); // Updated
            throw;
        }
    }

    // Print a standard message
    inline void print_message(const std::string &message)
    {
        std::cout << "[kit] " << message << std::endl;
    }

    // Print an error message
    inline void print_error(const std::string &message)
    {
        std::cerr << "[kit] Error: " << message << std::endl;
    }

    inline void debug(const std::string &message)
    {
#ifdef DEBUG
        std::cout << "[DEBUG] " << message << std::endl;
#else
        (void)message; // Suppress unused parameter warning
#endif
    }

    // Create a file with optional content
    inline void create_file(const std::string &path, const std::string &content = "")
    {
        try
        {
            std::ofstream file(path);
            if (!file)
            {
                throw std::runtime_error("Failed to create file: " + path);
            }
            file << content;
        }
        catch (const std::exception &e)
        {
            error_handler::print_error("Error creating file: " + std::string(e.what()));
        }
    }

    // Initialize the repository
    inline void initialize_repository()
    {
        if (!std::filesystem::exists(".kit"))
        {
            std::filesystem::create_directory(".kit");
            create_file(".kit/HEAD", "");  // Create an empty HEAD file
            create_file(".kit/index", ""); // Create an empty index file
        }
    }

    // Ensure the repository is initialized
    inline bool ensure_repository_initialized()
    {
        if (!std::filesystem::exists(".kit"))
        {
            print_error("No kit repository found. Please initialize a repository first.");
            return false;
        }
        return true;
    }

    // Check if there are staged files
    inline bool has_staged_files()
    {
        return std::filesystem::exists(INDEX_FILE) && std::filesystem::file_size(INDEX_FILE) > 0;
    }

    // Update the HEAD file with the given commit hash
    inline void update_head(const std::string &commit_hash)
    {
        try
        {
            kit_utils::create_file(HEAD_FILE, commit_hash);
            std::cout << "[DEBUG] HEAD file updated to: " << commit_hash << "\n";
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to update HEAD: " + std::string(e.what()));
            throw;
        }
    }

    inline std::string get_parent_commit(const std::string &commit_hash)
    {
        std::string commit_path = OBJECTS_DIR + "/" + commit_hash;
        if (!std::filesystem::exists(commit_path))
        {
            throw std::runtime_error("Commit not found: " + commit_hash);
        }

        std::ifstream commit_file(commit_path + "/commit");
        if (!commit_file)
        {
            throw std::runtime_error("Failed to open commit file: " + commit_path + "/commit");
        }

        std::string parent_commit;
        std::getline(commit_file, parent_commit); // Assume the second line contains the parent commit hash
        return parent_commit;
    }

    // Find the common ancestor of two commits
    inline std::string find_common_ancestor(const std::string &commit1, const std::string &commit2)
    {
        std::unordered_set<std::string> commit1_ancestors;

        std::string current = commit1;
        while (!current.empty())
        {
            commit1_ancestors.insert(current);
            current = get_parent_commit(current); // Helper function to get the parent commit
        }

        current = commit2;
        while (!current.empty())
        {
            if (commit1_ancestors.count(current))
            {
                return current; // Found the common ancestor
            }
            current = get_parent_commit(current);
        }

        return ""; // No common ancestor found
    }

    // Create a commit with files and a message
    inline std::string create_commit(const std::unordered_map<std::string, std::string> &files, const std::string &message)
    {
        try
        {
            // Prepare commit content
            std::ostringstream commit_content;
            commit_content << "Commit message: " << message << "\n";

            // Add files to the commit
            for (const auto &[file_name, file_content] : files)
            {
                commit_content << "File: " << file_name << "\n";
                commit_content << file_content << "\n";
            }

            // Generate a hash for the commit
            std::string commit_hash = hash_object::compute_sha1(commit_content.str());
            std::cout << "[DEBUG] Generated commit hash: " << commit_hash << "\n";

            // Write the commit to the objects directory
            std::string commit_path = OBJECTS_DIR + "/" + commit_hash;
            kit_utils::create_file(commit_path, commit_content.str());
            std::cout << "[DEBUG] Commit written to: " << commit_path << "\n";

            // Update HEAD to point to the new commit
            kit_utils::update_head(commit_hash);
            std::cout << "[DEBUG] HEAD updated to: " << commit_hash << "\n";

            kit_utils::print_message("Commit created successfully with message: " + message);
            return commit_hash;
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to create commit: " + std::string(e.what()));
            return ""; // Return an empty string on failure
        }
    }

    // Retrieve files from a specific commit
    inline std::unordered_map<std::string, std::string> get_commit_files(const std::string &commit_hash)
    {
        std::unordered_map<std::string, std::string> files;
        std::string commit_path = OBJECTS_DIR + "/" + commit_hash;

        if (!std::filesystem::exists(commit_path))
        {
            throw std::runtime_error("Commit not found: " + commit_hash);
        }

        for (const auto &entry : std::filesystem::directory_iterator(commit_path))
        {
            if (entry.is_regular_file())
            {
                files[entry.path().filename().string()] = kit_utils::read_file(entry.path().string());
            }
        }

        return files;
    }

    inline std::unordered_map<std::string, std::string> get_working_directory_files()
    {
        std::unordered_map<std::string, std::string> files;

        for (const auto &entry : std::filesystem::recursive_directory_iterator("."))
        {
            if (entry.is_regular_file() && entry.path().string().find(KIT_DIR) == std::string::npos)
            {
                std::ifstream file(entry.path());
                std::stringstream buffer;
                buffer << file.rdbuf();
                files[entry.path().filename().string()] = buffer.str();
            }
        }

        return files;
    }

    // Get differences between the working directory and a specific commit
    inline std::vector<std::string> get_differences(const std::string &commit_hash)
    {
        std::vector<std::string> differences;

        try
        {
            // Retrieve files from the specified commit
            std::unordered_map<std::string, std::string> commit_files;
            if (!commit_hash.empty())
            {
                commit_files = kit_utils::get_commit_files(commit_hash);
            }
            else
            {
                throw std::runtime_error("Commit hash is empty.");
            }

            // Retrieve files from the working directory
            auto working_files = kit_utils::get_working_directory_files();

            // Compare files between the commit and the working directory
            for (const auto &[file_name, commit_content] : commit_files)
            {
                if (working_files.find(file_name) == working_files.end())
                {
                    differences.push_back("File deleted: " + file_name);
                }
                else if (working_files[file_name] != commit_content)
                {
                    differences.push_back("File modified: " + file_name);
                }
            }

            // Check for new files in the working directory
            for (const auto &[file_name, working_content] : working_files)
            {
                if (commit_files.find(file_name) == commit_files.end())
                {
                    differences.push_back("File added: " + file_name);
                }
            }

            if (differences.empty())
            {
                differences.push_back("No differences found.");
            }
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to get differences: " + std::string(e.what()));
        }

        return differences;
    }

    inline std::unordered_map<std::string, std::string> perform_three_way_merge(
        const std::string &base_commit,
        const std::string &current_commit,
        const std::string &target_commit)
    {
        // Example logic for merging files (you can replace this with your actual logic)
        std::unordered_map<std::string, std::string> merged_files;

        // Retrieve files from each commit
        auto base_files = get_commit_files(base_commit);
        auto current_files = get_commit_files(current_commit);
        auto target_files = get_commit_files(target_commit);

        // Perform a simple merge (replace this with your actual merge logic)
        for (const auto &[file, content] : base_files)
        {
            if (current_files[file] == target_files[file])
            {
                merged_files[file] = current_files[file]; // No conflict
            }
            else
            {
                merged_files[file] = "<<<<<<< CURRENT\n" + current_files[file] +
                                     "=======\n" + target_files[file] +
                                     ">>>>>>>\n"; // Simple conflict marker
            }
        }

        return merged_files;
    }
} // namespace kit_utils

#endif // KIT_UTILS_HPP
