#ifndef MERGE_HPP
#define MERGE_HPP

#include <string>
#include <filesystem>
#include "../utils/kit_utils.hpp"

namespace kit_vcs
{
    inline bool merge_branch(const std::string &branch_name)
    {
        if (!kit_utils::ensure_repository_initialized())
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

            // TODO: Implement actual merge logic (e.g., combining commits, resolving conflicts)
            kit_utils::print_message("Merged branch: " + branch_name);
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
