#ifndef MERGE_HPP
#define MERGE_HPP

#include <string>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include "../utils/kit_utils.hpp"
#include "../utils/constants.hpp"

namespace kit_vcs
{

    inline void update_head(const std::string &commit_hash)
    {
        kit_utils::create_file(HEAD_FILE, commit_hash); // Use the correct namespace
    }

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
                std::ifstream file(entry.path());
                std::stringstream buffer;
                buffer << file.rdbuf();
                files[entry.path().filename().string()] = buffer.str();
            }
        }

        return files;
    }

    inline bool merge_branch(const std::string &branch_name)
    {
        if (!kit_utils::ensure_repository_initialized())
        {
            return false;
        }

        try
        {
            // Check if the branch exists
            std::string branch_path = std::string(HEADS_DIR) + "/" + branch_name;
            if (!std::filesystem::exists(branch_path))
            {
                kit_utils::print_error("Branch does not exist: " + branch_name);
                return false;
            }

            // Read the current branch and target branch commit hashes
            std::string current_commit, target_commit;
            if (std::ifstream head_file(HEAD_FILE); head_file)
            {
                std::getline(head_file, current_commit);
            }
            if (std::ifstream branch_file(branch_path); branch_file)
            {
                std::getline(branch_file, target_commit);
            }

            // Check if the branches are already merged
            if (current_commit == target_commit)
            {
                kit_utils::print_message("Branches are already up to date.");
                return true;
            }

            // Perform a simple three-way merge
            std::string base_commit = kit_utils::find_common_ancestor(current_commit, target_commit);
            if (base_commit.empty())
            {
                kit_utils::print_error("No common ancestor found. Cannot merge.");
                return false;
            }

            // Simulate merging changes (e.g., combining commits)
            std::unordered_map<std::string, std::string> base_files = get_commit_files(base_commit);
            std::unordered_map<std::string, std::string> current_files = get_commit_files(current_commit);
            std::unordered_map<std::string, std::string> target_files = get_commit_files(target_commit);

            std::unordered_map<std::string, std::string> merged_files;
            bool conflict_detected = false;

            for (const auto &[file, base_content] : base_files)
            {
                std::string current_content = current_files[file];
                std::string target_content = target_files[file];

                if (current_content == target_content)
                {
                    merged_files[file] = current_content; // No changes
                }
                else if (current_content == base_content)
                {
                    merged_files[file] = target_content; // Accept target changes
                }
                else if (target_content == base_content)
                {
                    merged_files[file] = current_content; // Accept current changes
                }
                else
                {
                    // Conflict detected
                    conflict_detected = true;
                    merged_files[file] = "<<<<<<< HEAD\n" + current_content + "\n=======\n" + target_content + "\n>>>>>>>\n";
                    kit_utils::print_error("Conflict detected in file: " + file);
                }
            }

            // Handle new files in the target branch
            for (const auto &[file, target_content] : target_files)
            {
                if (base_files.find(file) == base_files.end() && current_files.find(file) == current_files.end())
                {
                    merged_files[file] = target_content;
                }
            }

            // Write the merged files to a new commit
            std::string new_commit_hash = kit_utils::create_commit(merged_files, "Merged branch " + branch_name);
            if (new_commit_hash.empty())
            {
                kit_utils::print_error("Failed to create merge commit.");
                return false;
            }
            kit_utils::update_head(new_commit_hash);

            if (conflict_detected)
            {
                kit_utils::print_message("Merge completed with conflicts. Resolve conflicts and commit.");
                return false;
            }

            kit_utils::print_message("Merge completed successfully.");
            return true;
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to merge branches: " + std::string(e.what()));
            return false;
        }
    }
} // namespace kit_vcs

#endif // MERGE_HPP
