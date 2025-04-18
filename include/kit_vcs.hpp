#ifndef KIT_VCS_HPP
#define KIT_VCS_HPP

#include "commands/branch.hpp"
#include "commands/checkout.hpp"
#include "commands/commit.hpp"
#include "commands/diff.hpp"
#include "commands/merge.hpp"
#include "commands/reset.hpp"
#include "commands/stash.hpp"
#include "commands/status.hpp"
#include "utils/constants.hpp"
#include "utils/kit_utils.hpp"
#include "utils/error_handler.hpp"
#include "utils/hash_object.hpp"
#include "version.hpp"

namespace kit_vcs
{
    // Initialize a new repository
    inline bool initialize_repository()
    {
        if (std::filesystem::exists(KIT_DIR))
        {
            error_handler::print_error("Repository already initialized.");
            return false;
        }

        try
        {
            std::filesystem::create_directories(HEADS_DIR);
            std::filesystem::create_directories(OBJECTS_DIR);
            kit_utils::create_file(HEAD_FILE, "ref: refs/heads/master\n");
            kit_utils::create_file(INDEX_FILE);
            kit_utils::print_message("Repository initialized successfully.");
            return true;
        }
        catch (const std::exception &e)
        {
            error_handler::print_error("Failed to initialize repository: " + std::string(e.what()));
            return false;
        }
    }

    // Stage a file for commit
    inline bool stage_file(const std::string &file)
    {
        if (!kit_utils::ensure_repository_initialized())
        {
            return false;
        }

        if (!std::filesystem::exists(file))
        {
            error_handler::print_error("File does not exist: " + file);
            return false;
        }

        try
        {
            std::ofstream index_file(INDEX_FILE, std::ios::app);
            if (!index_file)
            {
                error_handler::print_error("Failed to open index file.");
                return false;
            }
            index_file << file << "\n";
            kit_utils::print_message("File staged: " + file);
            return true;
        }
        catch (const std::exception &e)
        {
            error_handler::print_error("Failed to stage file: " + std::string(e.what()));
            return false;
        }
    }

    // Retrieve commit history
    inline std::vector<std::string> get_commit_history()
    {
        if (!kit_utils::ensure_repository_initialized())
        {
            return {};
        }

        std::vector<std::string> history;
        try
        {
            std::ifstream head_file(HEAD_FILE);
            if (!head_file)
            {
                error_handler::print_error("Failed to open HEAD file.");
                return {};
            }

            std::string current_commit;
            std::getline(head_file, current_commit);

            while (!current_commit.empty())
            {
                std::string commit_path = OBJECTS_DIR + "/" + current_commit;
                if (!std::filesystem::exists(commit_path))
                {
                    break;
                }

                std::ifstream commit_file(commit_path);
                if (!commit_file)
                {
                    error_handler::print_error("Failed to open commit file: " + commit_path);
                    break;
                }

                std::string commit_message;
                std::getline(commit_file, commit_message);

                history.push_back(current_commit + ": " + commit_message);

                // Assume the second line in the commit file contains the parent commit hash
                std::getline(commit_file, current_commit);
            }
        }
        catch (const std::exception &e)
        {
            error_handler::print_error("Failed to retrieve commit history: " + std::string(e.what()));
        }

        return history;
    }

    // Show the version of kit-vcs
    inline void show_version()
    {
        std::cout << "kit-vcs version " << KIT_VCS_VERSION << std::endl;
    }
} // namespace kit_vcs

#endif // KIT_VCS_HPP
