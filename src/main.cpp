#include <iostream>
#include <cxxopts.hpp>
#include "hash_object.hpp" // Include the hash_object header
#include "kit_utils.hpp"   // Include the utility functions

// Function to display the logo
void display_logo()
{
    std::cout << R"(
     🦊
    KIT-VCS
    A minimal version control system
    )" << std::endl;
}

int main(int argc, char *argv[])
{
    // Define CLI options
    cxxopts::Options options("kit", "Kit - A cute minimal git clone");

    options.add_options()("init", "Initialize a new kit repository")("add", "Add file(s) to the staging area", cxxopts::value<std::vector<std::string>>())("commit", "Commit staged files", cxxopts::value<std::string>())("log", "Show commit history")("status", "Show repository status")("stash", "Stash changes temporarily")("branch", "Manage branches")("checkout", "Switch branches", cxxopts::value<std::string>())("merge", "Merge branches", cxxopts::value<std::string>())("reset", "Reset to a specific commit", cxxopts::value<std::string>())("diff", "Show differences between commits or the working directory")("h,help", "Print help");

    // Parse command-line arguments
    auto result = options.parse(argc, argv);

    // Display help if no arguments or help flag is provided
    if (result.count("help") || argc == 1)
    {
        std::cout << options.help() << std::endl;
        return 0;
    }

    // Handle `init` command
    if (result.count("init"))
    {
        display_logo();
        print_message("Initializing kit repository...");
        // TODO: Implement repository initialization logic
    }

    // Handle `add` command
    if (result.count("add"))
    {
        for (const auto &file : result["add"].as<std::vector<std::string>>())
        {
            print_message("Adding file: " + file);
            // TODO: Implement logic to stage files
        }
    }

    // Handle `commit` command
    if (result.count("commit"))
    {
        std::string message = result["commit"].as<std::string>();
        print_message("Committing staged files with message: " + message);
        // TODO: Implement commit logic
    }

    // Handle `log` command
    if (result.count("log"))
    {
        print_message("Showing commit history...");
        // TODO: Implement log functionality
    }

    // Handle `status` command
    if (result.count("status"))
    {
        print_message("Showing repository status...");
        // TODO: Implement status functionality
    }

    // Handle `stash` command
    if (result.count("stash"))
    {
        print_message("Stashing changes...");
        // TODO: Implement stash logic
    }

    // Handle `branch` command
    if (result.count("branch"))
    {
        print_message("Managing branches...");
        // TODO: Implement branch functionality
    }

    // Handle `checkout` command
    if (result.count("checkout"))
    {
        std::string branch = result["checkout"].as<std::string>();
        print_message("Switching to branch: " + branch);
        // TODO: Implement checkout functionality
    }

    // Handle `merge` command
    if (result.count("merge"))
    {
        std::string branch = result["merge"].as<std::string>();
        print_message("Merging branch: " + branch);
        // TODO: Implement merge functionality
    }

    // Handle `reset` command
    if (result.count("reset"))
    {
        std::string commit = result["reset"].as<std::string>();
        print_message("Resetting to commit: " + commit);
        // TODO: Implement reset functionality
    }

    // Handle `diff` command
    if (result.count("diff"))
    {
        print_message("Showing differences...");
        // TODO: Implement diff functionality
    }

    return 0;
}
