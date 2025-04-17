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
#include "version.hpp"

namespace kit_vcs
{
    inline void initialize_repository()
    {
        if (std::filesystem::exists(KIT_DIR))
        {
            kit_utils::print_error("Repository already initialized.");
            return;
        }

        try
        {
            std::filesystem::create_directories(HEADS_DIR);
            std::filesystem::create_directories(OBJECTS_DIR);
            kit_utils::create_file(HEAD_FILE, "ref: refs/heads/master\n");
            kit_utils::create_file(INDEX_FILE);
            kit_utils::print_message("Repository initialized successfully.");
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to initialize repository: " + std::string(e.what()));
        }
    }

    inline void stage_file(const std::string &file)
    {
        if (!kit_utils::ensure_repository_initialized())
        {
            return;
        }

        if (!std::filesystem::exists(file))
        {
            kit_utils::print_error("File does not exist: " + file);
            return;
        }

        try
        {
            std::ofstream index_file(INDEX_FILE, std::ios::app);
            index_file << file << "\n";
            index_file.close();
            kit_utils::print_message("File staged: " + file);
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to stage file: " + std::string(e.what()));
        }
    }

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
            std::string current_commit;
            std::getline(head_file, current_commit);
            head_file.close();

            while (!current_commit.empty())
            {
                std::string commit_path = OBJECTS_DIR + "/" + current_commit;
                if (!std::filesystem::exists(commit_path))
                {
                    break;
                }

                std::ifstream commit_file(commit_path);
                std::string commit_message;
                std::getline(commit_file, commit_message);
                commit_file.close();

                history.push_back(current_commit + ": " + commit_message);

                // For simplicity, assume the commit file contains a reference to the parent commit
                std::getline(commit_file, current_commit);
            }
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to retrieve commit history: " + std::string(e.what()));
        }

        return history;
    }

    inline void show_version()
    {
        std::cout << "kit-vcs version " << KIT_VCS_VERSION << std::endl;
    }
} // namespace kit_vcs

#endif // KIT_VCS_HPP
