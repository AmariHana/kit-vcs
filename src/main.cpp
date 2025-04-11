#include <iostream>
#include <cxxopts.hpp>
#include <filesystem>
#include <fstream>
#include <string>
#include "kit_vcs.hpp"

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
    if (!kit_utils::ensure_repository_initialized())
        return;

    for (const auto &file : files)
    {
        if (!std::filesystem::exists(file))
        {
            kit_utils::print_error("File does not exist: " + file);
            continue;
        }

        if (!kit_vcs::stage_file(file))
        {
            kit_utils::print_error("Failed to stage file: " + file);
        }
        else
        {
            kit_utils::print_message("File staged successfully: " + file);
        }
    }
}

void handle_commit(const std::string &message)
{
    if (!kit_utils::ensure_repository_initialized())
        return;

    if (!kit_vcs::create_commit(message))
    {
        kit_utils::print_error("Failed to commit.");
    }
}

void handle_status()
{
    if (!kit_utils::ensure_repository_initialized())
        return;

    auto status = kit_vcs::get_repository_status();
    if (status.empty())
    {
        kit_utils::print_message("Working directory clean. Nothing to commit.");
    }
    else
    {
        for (const auto &file : status)
        {
            std::cout << file << std::endl;
        }
    }
}

int main(int argc, char *argv[])
{
    try
    {
        cxxopts::Options options("kit", "Kit - A cute minimal git clone");

        options.add_options()("init", "Initialize a new kit repository")("add", "Add file(s) to the staging area", cxxopts::value<std::vector<std::string>>())("commit", "Commit staged files", cxxopts::value<std::string>())("status", "Show repository status")("h,help", "Print help");

        auto result = options.parse(argc, argv);

        if (result.count("help") || argc == 1)
        {
            std::cout << options.help() << std::endl;
            return 0;
        }

        if (result.count("init"))
            handle_init();
        if (result.count("add"))
        {
            if (result["add"].as<std::vector<std::string>>().empty())
            {
                kit_utils::print_error("The '--add' option requires at least one file as an argument.");
                return 1;
            }
            handle_add(result["add"].as<std::vector<std::string>>());
        }
        if (result.count("commit"))
        {
            if (result["commit"].as<std::string>().empty())
            {
                kit_utils::print_error("The '--commit' option requires a commit message.");
                return 1;
            }
            handle_commit(result["commit"].as<std::string>());
        }
        if (result.count("status"))
            handle_status();
    }
    catch (const cxxopts::exceptions::invalid_option_syntax &e)
    {
        kit_utils::print_error(std::string("Invalid option syntax: ") + e.what());
        return 1;
    }
    catch (const std::exception &e)
    {
        kit_utils::print_error(std::string("An error occurred: ") + e.what());
        return 1;
    }

    return 0;
}
