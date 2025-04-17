#include <iostream>
#include <cxxopts.hpp>
#include "cli.hpp"

int main(int argc, char *argv[])
{
    try
    {
        // Define CLI options
        cxxopts::Options options("kit", "Kit - A minimal version control system");

        options.add_options()("init", "Initialize a new kit repository")("add", "Add file(s) to the staging area", cxxopts::value<std::vector<std::string>>())("commit", "Commit staged files", cxxopts::value<std::string>())("status", "Show repository status")("log", "Show commit history")("stash", "Stash changes temporarily")("branch", "Manage branches")("checkout", "Switch branches", cxxopts::value<std::string>())("merge", "Merge branches", cxxopts::value<std::string>())("reset", "Reset to a specific commit", cxxopts::value<std::string>())("diff", "Show differences between commits or the working directory")("version", "Show the version of kit-vcs")("h,help", "Print help");

        auto result = options.parse(argc, argv);

        // Handle help command or no arguments
        if (result.count("help") || argc == 1)
        {
            cli::display_help_screen();
            return 0;
        }

        // Handle version command
        if (result.count("version"))
        {
            cli::handle_version();
            return 0;
        }

        // Handle other commands
        if (result.count("init"))
        {
            cli::handle_init();
        }
        if (result.count("add"))
        {
            cli::handle_add(result["add"].as<std::vector<std::string>>());
        }
        if (result.count("commit"))
        {
            cli::handle_commit(result["commit"].as<std::string>());
        }
        if (result.count("status"))
        {
            cli::handle_status();
        }
        if (result.count("log"))
        {
            cli::handle_log();
        }
        if (result.count("stash"))
        {
            cli::handle_stash();
        }
        if (result.count("branch"))
        {
            cli::handle_branch();
        }
        if (result.count("checkout"))
        {
            cli::handle_checkout(result["checkout"].as<std::string>());
        }
        if (result.count("merge"))
        {
            cli::handle_merge(result["merge"].as<std::string>());
        }
        if (result.count("reset"))
        {
            cli::handle_reset(result["reset"].as<std::string>());
        }
        if (result.count("diff"))
        {
            cli::handle_diff();
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
