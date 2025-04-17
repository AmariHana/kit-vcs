#ifndef STATUS_HPP
#define STATUS_HPP

#include <string>
#include <vector>
#include <filesystem>
#include "../utils/constants.hpp"
#include "../utils/kit_utils.hpp"

namespace kit_vcs
{
    inline std::vector<std::string> get_repository_status()
    {
        std::vector<std::string> status;

        if (!kit_utils::ensure_repository_initialized())
        {
            return status;
        }

        try
        {
            // Check if there are staged files
            if (kit_utils::has_staged_files())
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
} // namespace kit_vcs

#endif // STATUS_HPP
