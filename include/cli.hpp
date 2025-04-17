#ifndef CLI_HPP
#define CLI_HPP

#include <iostream>
#include <vector>
#include <string>
#include <cxxopts.hpp>
#include "kit_vcs.hpp"
#include "version.hpp"
#include "utils/error_handler.hpp" // Corrected include path

namespace cli
{
    // Display the welcome screen
    void display_welcome_screen()
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
    void display_help_screen()
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
  version       Show the version of kit-vcs
  help          Show this help message
)" << std::endl;
    }

    // Command handlers
    void handle_init()
    {
        display_welcome_screen();
        kit_vcs::initialize_repository();
    }

    void handle_add(const std::vector<std::string> &files)
    {
        for (const auto &file : files)
        {
            if (!std::filesystem::exists(file))
            {
                error_handler::print_error("File does not exist: " + file);
                continue;
            }
            kit_vcs::stage_file(file);
        }
    }

    void handle_commit(const std::string &message)
    {
        if (message.empty())
        {
            error_handler::print_error("Commit message cannot be empty.");
            return;
        }
        kit_vcs::create_commit(message);
    }

    void handle_status()
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

    void handle_log()
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

    void handle_stash()
    {
        if (!kit_vcs::stash_changes())
        {
            error_handler::print_error("Failed to stash changes.");
        }
    }

    void handle_branch()
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

    void handle_checkout(const std::string &branch)
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

    void handle_merge(const std::string &branch)
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

    void handle_reset(const std::string &commit)
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

    void handle_diff()
    {
        std::string commit_hash = "HEAD"; // Default to HEAD or specify the desired commit hash
        auto differences = kit_vcs::get_differences(commit_hash);
        if (differences.empty())
        {
            kit_utils::print_message("No differences to display.");
        }
        else
        {
            for (const auto &diff : differences)
            {
                std::cout << diff << std::endl;
            }
        }
    }

    void handle_version()
    {
        kit_vcs::show_version();
    }
} // namespace cli

#endif // CLI_HPP
