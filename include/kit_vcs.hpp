#ifndef KIT_VCS_HPP
#define KIT_VCS_HPP

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <sstream>
#include "kit_utils.hpp"
#include "hash_object.hpp"

namespace kit_vcs
{

    // Constants
    constexpr auto KIT_DIR = ".kit";
    constexpr auto OBJECTS_DIR = ".kit/objects";
    constexpr auto REFS_DIR = ".kit/refs";
    constexpr auto HEADS_DIR = ".kit/refs/heads";
    constexpr auto HEAD_FILE = ".kit/HEAD";
    constexpr auto INDEX_FILE = ".kit/index";

    // Utility Functions
    inline bool ensure_repository_initialized()
    {
        if (!std::filesystem::exists(KIT_DIR))
        {
            kit_utils::print_error("No kit repository found in this directory. Please initialize one first.");
            return false;
        }
        return true;
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

    // Repository Initialization
    inline void initialize_repository()
    {
        if (std::filesystem::exists(KIT_DIR))
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

    // Staging Area
    inline bool is_file_staged(const std::string &file_path)
    {
        std::ifstream index_file(INDEX_FILE);
        if (!index_file)
        {
            return false;
        }

        std::string line;
        while (std::getline(index_file, line))
        {
            if (line.find(file_path) != std::string::npos)
            {
                return true;
            }
        }
        return false;
    }

    inline bool stage_file(const std::string &file_path)
    {
        if (is_file_staged(file_path))
        {
            kit_utils::print_message("File is already staged: " + file_path);
            return false;
        }

        try
        {
            std::ifstream file(file_path, std::ios::binary);
            if (!file)
            {
                kit_utils::print_error("Failed to read file: " + file_path);
                return false;
            }

            std::ostringstream buffer;
            buffer << file.rdbuf();
            std::string content = buffer.str();

            std::string hash = hash_object::compute_sha1(content);

            std::ofstream index_file(INDEX_FILE, std::ios::app);
            if (!index_file)
            {
                kit_utils::print_error("Failed to open index file.");
                return false;
            }
            index_file << hash << " " << file_path << "\n";
            kit_utils::print_message("File staged successfully: " + file_path);
            return true;
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to stage file: " + std::string(e.what()));
            return false;
        }
    }

    inline bool has_staged_files()
    {
        std::ifstream index_file(INDEX_FILE);
        if (!index_file)
        {
            return false;
        }

        std::string line;
        while (std::getline(index_file, line))
        {
            if (!line.empty())
            {
                return true;
            }
        }
        return false;
    }

    inline void unstage_file(const std::string &file_path)
    {
        std::ifstream index_file(INDEX_FILE);
        if (!index_file)
        {
            kit_utils::print_error("Failed to open index file.");
            return;
        }

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(index_file, line))
        {
            if (line.find(file_path) == std::string::npos)
            {
                lines.push_back(line);
            }
        }
        index_file.close();

        std::ofstream new_index_file(INDEX_FILE);
        for (const auto &l : lines)
        {
            new_index_file << l << "\n";
        }
        new_index_file.close();
        kit_utils::print_message("File unstaged successfully: " + file_path);
    }
    inline void unstage_all_files()
    {
        std::ofstream new_index_file(INDEX_FILE);
        new_index_file.close();
        kit_utils::print_message("All files unstaged successfully.");
    }
    inline bool is_repository_initialized()
    {
        return std::filesystem::exists(KIT_DIR);
    }
    inline bool is_file_in_repository(const std::string &file_path)
    {
        return std::filesystem::exists(file_path) && file_path.find(KIT_DIR) == std::string::npos;
    }
    inline bool is_file_in_working_directory(const std::string &file_path)
    {
        return std::filesystem::exists(file_path) && file_path.find(KIT_DIR) == std::string::npos;
    }
    inline bool is_file_in_staging_area(const std::string &file_path)
    {
        std::ifstream index_file(INDEX_FILE);
        if (!index_file)
        {
            return false;
        }

        std::string line;
        while (std::getline(index_file, line))
        {
            if (line.find(file_path) != std::string::npos)
            {
                return true;
            }
        }
        return false;
    }
    inline bool is_file_in_commit(const std::string &file_path, const std::string &commit_hash)
    {
        std::string commit_path = std::string(OBJECTS_DIR) + "/" + commit_hash;
        if (!std::filesystem::exists(commit_path))
        {
            return false;
        }

        std::ifstream commit_file(commit_path);
        if (!commit_file)
        {
            return false;
        }

        std::string line;
        while (std::getline(commit_file, line))
        {
            if (line.find(file_path) != std::string::npos)
            {
                return true;
            }
        }
        return false;
    }
    inline bool is_file_in_stash(const std::string &file_path)
    {
        std::ifstream stash_file(INDEX_FILE);
        if (!stash_file)
        {
            return false;
        }

        std::string line;
        while (std::getline(stash_file, line))
        {
            if (line.find(file_path) != std::string::npos)
            {
                return true;
            }
        }
        return false;
    }

    // Stash Management
    inline bool stash_changes()
    {
        if (!ensure_repository_initialized())
        {
            return false;
        }

        try
        {
            std::string stash_path = std::string(KIT_DIR) + "/stash";
            std::ofstream stash_file(stash_path, std::ios::app);
            if (!stash_file)
            {
                kit_utils::print_error("Failed to open stash file.");
                return false;
            }

            // Add staged files to the stash
            std::ifstream index_file(INDEX_FILE);
            if (!index_file)
            {
                kit_utils::print_error("Failed to read index file.");
                return false;
            }

            std::string line;
            while (std::getline(index_file, line))
            {
                stash_file << line << "\n";
            }
            index_file.close();

            // Clear the index file after stashing
            create_file(INDEX_FILE);

            kit_utils::print_message("Changes stashed successfully.");
            return true;
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to stash changes: " + std::string(e.what()));
            return false;
        }
    }

    // Reset Operations
    inline bool reset_to_commit(const std::string &commit_hash)
    {
        if (!ensure_repository_initialized())
        {
            return false;
        }

        try
        {
            std::string commit_path = std::string(OBJECTS_DIR) + "/" + commit_hash;
            if (!std::filesystem::exists(commit_path))
            {
                kit_utils::print_error("Commit does not exist: " + commit_hash);
                return false;
            }

            // Reset HEAD to the specified commit
            create_file(HEAD_FILE, "ref: " + commit_hash + "\n");

            // Clear the index file
            create_file(INDEX_FILE);

            kit_utils::print_message("Repository reset to commit: " + commit_hash);
            return true;
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to reset to commit: " + std::string(e.what()));
            return false;
        }
    }

    // Commit Operations
    inline bool create_commit(const std::string &message)
    {
        if (!has_staged_files())
        {
            kit_utils::print_error("No files staged for commit.");
            return false;
        }

        try
        {
            // Logic to create a commit (e.g., write to objects directory)
            std::ostringstream commit_content;
            commit_content << "Commit message: " << message << "\n";

            // Add staged files to the commit
            std::ifstream index_file(INDEX_FILE);
            if (!index_file)
            {
                kit_utils::print_error("Failed to read index file.");
                return false;
            }

            std::string line;
            while (std::getline(index_file, line))
            {
                commit_content << line << "\n";
            }
            index_file.close();

            // Generate a hash for the commit
            std::string commit_hash = hash_object::compute_sha1(commit_content.str());

            // Write the commit to the objects directory
            std::string commit_path = std::string(OBJECTS_DIR) + "/" + commit_hash;
            create_file(commit_path, commit_content.str());

            // Update HEAD to point to the new commit
            create_file(HEAD_FILE, commit_hash + "\n");

            // Clear the index file after committing
            create_file(INDEX_FILE);

            kit_utils::print_message("Commit created successfully with message: " + message);
            return true;
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to create commit: " + std::string(e.what()));
            return false;
        }
    }

    inline std::vector<std::string> get_commit_history()
    {
        std::vector<std::string> commit_history;

        if (!ensure_repository_initialized())
        {
            return commit_history;
        }

        try
        {
            std::ifstream head_file(HEAD_FILE);
            if (!head_file)
            {
                kit_utils::print_error("Failed to open HEAD file.");
                return commit_history;
            }

            std::string line;
            while (std::getline(head_file, line))
            {
                commit_history.push_back(line);
            }
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to retrieve commit history: " + std::string(e.what()));
        }

        return commit_history;
    }

    // Repository Status
    inline std::vector<std::string> get_repository_status()
    {
        std::vector<std::string> status;

        if (!ensure_repository_initialized())
        {
            return status;
        }

        try
        {
            // Check if there are staged files
            if (has_staged_files())
            {
                status.push_back("Staged files are ready for commit.");
            }
            else
            {
                status.push_back("Working directory clean. Nothing to commit.");
            }

            // Check for untracked files
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

    // Branch Management
    inline bool create_branch(const std::string &branch_name)
    {
        if (!ensure_repository_initialized())
        {
            return false;
        }

        try
        {
            std::string branch_path = std::string(HEADS_DIR) + "/" + branch_name;
            if (std::filesystem::exists(branch_path))
            {
                kit_utils::print_error("Branch already exists: " + branch_name);
                return false;
            }

            // Get the current HEAD commit hash
            std::ifstream head_file(HEAD_FILE);
            std::string current_commit;
            if (head_file)
            {
                std::getline(head_file, current_commit);
            }
            head_file.close();

            // Write the current commit hash (or empty) to the new branch file
            create_file(branch_path, current_commit);

            kit_utils::print_message("Branch created successfully: " + branch_name);
            return true;
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to create branch: " + std::string(e.what()));
            return false;
        }
    }

    inline bool switch_branch(const std::string &branch_name)
    {
        if (!ensure_repository_initialized())
        {
            return false;
        }

        try
        {
            std::string branch_path = std::string(HEADS_DIR) + "/" + branch_name;
            if (!std::filesystem::exists(branch_path))
            {
                kit_utils::print_error("Branch does not exist: " + branch_name);
                return false;
            }

            // Update the HEAD file to point to the new branch
            create_file(HEAD_FILE, "ref: refs/heads/" + branch_name + "\n");
            kit_utils::print_message("Switched to branch: " + branch_name);
            return true;
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to switch branches: " + std::string(e.what()));
            return false;
        }
    }

    inline std::vector<std::string> list_branches()
    {
        std::vector<std::string> branches;

        if (!ensure_repository_initialized())
        {
            return branches;
        }

        try
        {
            for (const auto &entry : std::filesystem::directory_iterator(HEADS_DIR))
            {
                if (entry.is_regular_file())
                {
                    branches.push_back(entry.path().filename().string());
                }
            }

            if (branches.empty())
            {
                kit_utils::print_message("No branches found.");
            }
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to list branches: " + std::string(e.what()));
        }

        return branches;
    }

    inline bool merge_branch(const std::string &branch_name)
    {
        if (!ensure_repository_initialized())
            return false;

        std::string branch_path = std::string(HEADS_DIR) + "/" + branch_name;
        if (!std::filesystem::exists(branch_path))
        {
            kit_utils::print_error("Branch does not exist: " + branch_name);
            return false;
        }

        try
        {
            // Placeholder for actual merge logic
            kit_utils::print_message("Merged branch: " + branch_name);
            return true;
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to merge branches: " + std::string(e.what()));
            return false;
        }
    }

    // Diff Operations
    inline std::vector<std::string> get_differences(const std::string &commit_hash)
    {
        std::vector<std::string> differences;

        if (!ensure_repository_initialized())
        {
            return differences;
        }

        try
        {
            // Placeholder logic for computing differences
            differences.push_back("Difference computation is not yet implemented for commit: " + commit_hash);
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to get differences: " + std::string(e.what()));
        }

        return differences;
    }

    // Log Operations
    inline std::vector<std::string> get_log()
    {
        std::vector<std::string> log_entries;

        if (!ensure_repository_initialized())
        {
            return log_entries;
        }

        try
        {
            std::ifstream head_file(HEAD_FILE);
            if (!head_file)
            {
                kit_utils::print_error("Failed to open HEAD file.");
                return log_entries;
            }

            std::string line;
            while (std::getline(head_file, line))
            {
                log_entries.push_back(line);
            }
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to retrieve log: " + std::string(e.what()));
        }

        return log_entries;
    }

    // File Operations
    inline void delete_file(const std::string &file_path)
    {
        if (std::filesystem::remove(file_path))
        {
            kit_utils::print_message("File deleted successfully: " + file_path);
        }
        else
        {
            kit_utils::print_error("Failed to delete file: " + file_path);
        }
    }

    inline void rename_file(const std::string &old_name, const std::string &new_name)
    {
        try
        {
            std::filesystem::rename(old_name, new_name);
            kit_utils::print_message("File renamed successfully: " + old_name + " to " + new_name);
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to rename file: " + old_name + ". Error: " + std::string(e.what()));
        }
    }

    inline void copy_file(const std::string &source, const std::string &destination)
    {
        if (std::filesystem::copy_file(source, destination))
        {
            kit_utils::print_message("File copied successfully: " + source + " to " + destination);
        }
        else
        {
            kit_utils::print_error("Failed to copy file: " + source);
        }
    }

    inline void move_file(const std::string &source, const std::string &destination)
    {
        try
        {
            std::filesystem::rename(source, destination);
            kit_utils::print_message("File moved successfully: " + source + " to " + destination);
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to move file: " + source + ". Error: " + std::string(e.what()));
        }
    }

    // Version Information
    inline std::string get_version()
    {
        return "kit-vcs version 1.0.0";
    }

    inline void show_version()
    {
        kit_utils::print_message(get_version());
    }

} // namespace kit_vcs

#endif // KIT_VCS_HPP
