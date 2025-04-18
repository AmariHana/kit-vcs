#ifndef CLI_HPP
#define CLI_HPP

#include <iostream>
#include <vector>
#include <string>
#include <cxxopts.hpp>
#include "../kit_vcs.hpp"
#include "../version.hpp"
#include "../utils/error_handler.hpp"
#include "../utils/kit_utils.hpp"

namespace cli
{
    // Display the welcome screen
    inline void display_welcome_screen()
    {
        std::cout << R"(
         🦊
        KIT-VCS
        A minimal version control system
        )" << std::endl;
        std::cout << "Version: " << KIT_VCS_VERSION << "\n"
                  << std::endl;
        std::cout << "Type `kit --help` to see available commands.\n"
                  << std::endl;
    }

    // Display the help screen
    inline void display_help_screen()
    {
        std::cout << R"(
Usage: kit [command] [options]

Commands:
  init          Initialize a new kit repository
  add           Add file(s) to the staging area
  commit        Commit staged files
  status        Show repository status
  log           Show commit history
  stash         Stash changes temporarily
  branch        Manage branches
  checkout      Switch branches
  merge         Merge branches
  reset         Reset to a specific commit
  diff          Show differences between commits or the working directory
  visualize     Visualize the repository structure
  version       Show the version of kit-vcs
  help          Show this help message
)" << std::endl;
    }

    // Visualize the repository structure
    inline void handle_visualize()
    {
        auto branches = kit_vcs::list_branches();
        auto commit_history = kit_vcs::get_commit_history();

        if (branches.empty())
        {
            kit_utils::print_message("No branches found to visualize.");
            return;
        }

        std::cout << "\nRepository Visualization:\n"
                  << std::endl;

        // Display branches
        std::cout << "Branches:" << std::endl;
        for (const auto &branch : branches)
        {
            std::cout << "  - " << branch << std::endl;
        }

        // Display commit history
        if (!commit_history.empty())
        {
            std::cout << "\nCommit History:" << std::endl;
            for (const auto &commit : commit_history)
            {
                std::cout << "  * " << commit << std::endl;
            }
        }
        else
        {
            std::cout << "\nNo commits found in the repository." << std::endl;
        }

        std::cout << "\nVisualization complete.\n"
                  << std::endl;
    }

    // Handle the `init` command
    inline void handle_init()
    {
        display_welcome_screen();
        if (!kit_vcs::initialize_repository())
        {
            error_handler::print_error("Failed to initialize repository.");
        }
    }

    // Handle the `add` command
    inline void handle_add(const std::vector<std::string> &files)
    {
        for (const auto &file : files)
        {
            if (!std::filesystem::exists(file))
            {
                error_handler::print_error("File does not exist: " + file);
                continue;
            }
            if (!kit_vcs::stage_file(file))
            {
                error_handler::print_error("Failed to stage file: " + file);
            }
        }
    }

    // Handle the `commit` command
    inline void handle_commit(const std::string &message)
    {
        if (message.empty())
        {
            error_handler::print_error("Commit message cannot be empty.");
            return;
        }
        if (!kit_vcs::create_commit(message))
        {
            error_handler::print_error("Failed to create commit.");
        }
    }

    // Handle the `status` command
    inline void handle_status()
    {
        auto status = kit_vcs::get_repository_status();
        if (status.empty())
        {
            kit_utils::print_message("Working directory clean. Nothing to commit.");
        }
        else
        {
            for (const auto &line : status)
            {
                std::cout << line << std::endl;
            }
        }
    }

    // Handle the `log` command
    inline void handle_log()
    {
        auto commit_history = kit_vcs::get_commit_history();
        if (commit_history.empty())
        {
            kit_utils::print_message("No commits found in the repository.");
        }
        else
        {
            for (const auto &commit : commit_history)
            {
                std::cout << commit << std::endl;
            }
        }
    }

    // Handle the `stash` command
    inline void handle_stash()
    {
        if (!kit_vcs::stash_changes())
        {
            error_handler::print_error("Failed to stash changes.");
        }
    }

    // Handle the `branch` command
    inline void handle_branch()
    {
        auto branches = kit_vcs::list_branches();
        if (branches.empty())
        {
            kit_utils::print_message("No branches found.");
        }
        else
        {
            kit_utils::print_message("Available branches:");
            for (const auto &branch : branches)
            {
                std::cout << branch << std::endl;
            }
        }
    }

    // Handle the `checkout` command
    inline void handle_checkout(const std::string &branch)
    {
        if (branch.empty())
        {
            error_handler::print_error("Branch name cannot be empty.");
            return;
        }
        if (!kit_vcs::switch_branch(branch))
        {
            error_handler::print_error("Failed to switch to branch: " + branch);
        }
    }

    // Handle the `merge` command
    inline void handle_merge(const std::string &branch)
    {
        if (branch.empty())
        {
            error_handler::print_error("Branch name cannot be empty.");
            return;
        }
        if (!kit_vcs::merge_branch(branch))
        {
            error_handler::print_error("Failed to merge branch: " + branch);
        }
    }

    // Handle the `reset` command
    inline void handle_reset(const std::string &commit)
    {
        if (commit.empty())
        {
            error_handler::print_error("Commit hash cannot be empty.");
            return;
        }
        if (!kit_vcs::reset_to_commit(commit))
        {
            error_handler::print_error("Failed to reset to commit: " + commit);
        }
    }

    // Handle the `diff` command
    inline void handle_diff()
    {
        try
        {
            // Default to HEAD or specify the desired commit hash
            std::string commit_hash = "HEAD";

            // Retrieve differences
            auto differences = kit_vcs::get_differences(commit_hash);

            // Check if differences are empty
            if (differences.empty())
            {
                kit_utils::print_message("No differences to display.");
                return;
            }

            // Display differences
            std::cout << "Differences:\n";
            for (const auto &diff : differences)
            {
                std::cout << "  - " << diff << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to display differences: " + std::string(e.what()));
        }
    }

    // Handle the `version` command
    inline void handle_version()
    {
        kit_vcs::show_version();
    }
} // namespace cli

#endif // CLI_HPP
