#ifndef BRANCH_HPP
#define BRANCH_HPP

#include <string>
#include <vector>
#include <filesystem>
#include "../utils/constants.hpp"
#include "../utils/kit_utils.hpp"

namespace kit_vcs
{
    inline bool create_branch(const std::string &branch_name)
    {
        if (!kit_utils::ensure_repository_initialized())
        {
            return false;
        }

        try
        {
            std::string branch_path = HEADS_DIR + "/" + branch_name;
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
} // namespace kit_vcs

#endif // BRANCH_HPP
