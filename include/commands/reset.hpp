#ifndef RESET_HPP
#define RESET_HPP

#include <string>
#include <filesystem>
#include "../utils/constants.hpp"
#include "../utils/kit_utils.hpp"

namespace kit_vcs
{
    inline bool reset_to_commit(const std::string &commit_hash)
    {
        if (!kit_utils::ensure_repository_initialized())
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
            kit_utils::create_file(HEAD_FILE, commit_hash + "\n");

            // Clear the index file
            kit_utils::create_file(INDEX_FILE);

            kit_utils::print_message("Repository reset to commit: " + commit_hash);
            return true;
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to reset to commit: " + std::string(e.what()));
            return false;
        }
    }
} // namespace kit_vcs

#endif // RESET_HPP
