#ifndef BRANCH_HPP
#define BRANCH_HPP

#include <string>
#include <vector>
#include <filesystem>
#include "../utils/constants.hpp"
#include "../utils/kit_utils.hpp"

namespace kit_vcs
{
    // Create a new branch
    inline bool create_branch(const std::string &branch_name)
    {
        if (!kit_utils::ensure_repository_initialized())
        {
            return false;
        }

        try
        {
            std::string branch_path = HEADS_DIR + "/" + branch_name;

            // Check if the branch already exists
            if (std::filesystem::exists(branch_path))
            {
                kit_utils::print_error("Branch already exists: " + branch_name);
                return false;
            }

            // Read the current HEAD commit hash
            std::string current_commit;
            if (std::ifstream head_file(HEAD_FILE); head_file)
            {
                std::getline(head_file, current_commit);
            }

            // Create the branch file with the current commit hash
            kit_utils::create_file(branch_path, current_commit);
            kit_utils::print_message("Branch created successfully: " + branch_name);
            return true;
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to create branch: " + std::string(e.what()));
            return false;
        }
    }

    // List all branches
    inline std::vector<std::string> list_branches()
    {
        std::vector<std::string> branches;

        if (!kit_utils::ensure_repository_initialized())
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

    // Change to a different branch
    inline bool change_branch(const std::string &branch_name)
    {
        if (!kit_utils::ensure_repository_initialized())
        {
            return false;
        }

        try
        {
            std::string branch_path = HEADS_DIR + "/" + branch_name;

            // Check if the branch exists
            if (!std::filesystem::exists(branch_path))
            {
                kit_utils::print_error("Branch does not exist: " + branch_name);
                return false;
            }

            // Update the HEAD file to point to the branch
            kit_utils::create_file(HEAD_FILE, branch_name);
            kit_utils::print_message("Switched to branch: " + branch_name);
            return true;
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to switch branch: " + std::string(e.what()));
            return false;
        }
    }

    // Delete a branch
    inline bool delete_branch(const std::string &branch_name)
    {
        if (!kit_utils::ensure_repository_initialized())
        {
            return false;
        }

        try
        {
            std::string branch_path = HEADS_DIR + "/" + branch_name;

            // Check if the branch exists
            if (!std::filesystem::exists(branch_path))
            {
                kit_utils::print_error("Branch does not exist: " + branch_name);
                return false;
            }

            // Check if the branch is the currently checked-out branch
            std::string current_branch;
            if (std::ifstream head_file(HEAD_FILE); head_file)
            {
                std::getline(head_file, current_branch);
            }

            if (current_branch == branch_name)
            {
                kit_utils::print_error("Cannot delete the currently checked-out branch: " + branch_name);
                return false;
            }

            // Delete the branch file
            std::filesystem::remove(branch_path);
            kit_utils::print_message("Branch deleted successfully: " + branch_name);
            return true;
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to delete branch: " + std::string(e.what()));
            return false;
        }
    }
} // namespace kit_vcs

#endif // BRANCH_HPP
