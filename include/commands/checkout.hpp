#ifndef CHECKOUT_HPP
#define CHECKOUT_HPP

#include <string>
#include <filesystem>
#include "../utils/constants.hpp"
#include "../utils/kit_utils.hpp"

namespace kit_vcs
{
    inline bool switch_branch(const std::string &branch_name)
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

            // Update the HEAD file to point to the new branch
            kit_utils::create_file(HEAD_FILE, "ref: refs/heads/" + branch_name + "\n");
            kit_utils::print_message("Switched to branch: " + branch_name);
            return true;
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to switch branches: " + std::string(e.what()));
            return false;
        }
    }
} // namespace kit_vcs

#endif // CHECKOUT_HPP
