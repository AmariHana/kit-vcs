#include <iostream>
#include <cxxopts.hpp>
#include <filesystem>
#include <string>
#include "kit_vcs.hpp"
#include "version.hpp"
#include "error_handler.hpp"

// Function to display the logo
void display_logo()
{
    std::cout << R"(
     🦊
    KIT-VCS
    A minimal version control system
    )" << std::endl;
}

// Command handlers
void handle_init()
{
    display_logo();
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

int main(int argc, char *argv[])
{
    try
    {
        cxxopts::Options options("kit", "Kit - A minimal version control system");

        options.add_options()("init", "Initialize a new kit repository")("add", "Add file(s) to the staging area", cxxopts::value<std::vector<std::string>>())("commit", "Commit staged files", cxxopts::value<std::string>())("status", "Show repository status")("log", "Show commit history")("stash", "Stash changes temporarily")("branch", "Manage branches")("checkout", "Switch branches", cxxopts::value<std::string>())("merge", "Merge branches", cxxopts::value<std::string>())("reset", "Reset to a specific commit", cxxopts::value<std::string>())("diff", "Show differences between commits or the working directory")("version", "Show the version of kit-vcs")("h,help", "Print help");

        auto result = options.parse(argc, argv);

        if (result.count("help") || argc == 1)
        {
            std::cout << options.help() << std::endl;
            return 0;
        }

        if (result.count("version"))
        {
            handle_version();
            return 0;
        }

        if (result.count("init"))
        {
            handle_init();
        }

        if (result.count("add"))
        {
            handle_add(result["add"].as<std::vector<std::string>>());
        }

        if (result.count("commit"))
        {
            handle_commit(result["commit"].as<std::string>());
        }

        if (result.count("status"))
        {
            handle_status();
        }

        if (result.count("log"))
        {
            handle_log();
        }

        if (result.count("stash"))
        {
            handle_stash();
        }

        if (result.count("branch"))
        {
            handle_branch();
        }

        if (result.count("checkout"))
        {
            handle_checkout(result["checkout"].as<std::string>());
        }

        if (result.count("merge"))
        {
            handle_merge(result["merge"].as<std::string>());
        }

        if (result.count("reset"))
        {
            handle_reset(result["reset"].as<std::string>());
        }

        if (result.count("diff"))
        {
            handle_diff();
        }
    }
    catch (const cxxopts::exceptions::invalid_option_syntax &e)
    {
        error_handler::handle_exception(e, "Invalid option syntax");
        return 1;
    }
    catch (const std::exception &e)
    {
        error_handler::handle_exception(e, "An error occurred");
        return 1;
    }

    return 0;
}
