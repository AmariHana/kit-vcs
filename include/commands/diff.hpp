#ifndef DIFF_HPP
#define DIFF_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include "../utils/kit_utils.hpp"
#include "../utils/constants.hpp"

namespace kit_vcs
{
    // Get differences between the working directory and a specific commit
    inline std::vector<std::string> get_differences(const std::string &commit_hash)
    {
        std::vector<std::string> differences;

        try
        {
            // Validate the commit hash
            if (commit_hash.empty())
            {
                throw std::runtime_error("Commit hash is empty.");
            }

            // Retrieve files from the specified commit
            auto commit_files = kit_utils::get_commit_files(commit_hash);

            // Retrieve files from the working directory
            auto working_files = kit_utils::get_working_directory_files();

            // Compare files between the commit and the working directory
            for (const auto &[file_name, commit_content] : commit_files)
            {
                if (working_files.find(file_name) == working_files.end())
                {
                    differences.push_back("File deleted: " + file_name);
                }
                else if (working_files[file_name] != commit_content)
                {
                    differences.push_back("File modified: " + file_name);
                }
            }

            // Check for new files in the working directory
            for (const auto &[file_name, working_content] : working_files)
            {
                if (commit_files.find(file_name) == commit_files.end())
                {
                    differences.push_back("File added: " + file_name);
                }
            }

            // If no differences are found, indicate that explicitly
            if (differences.empty())
            {
                differences.push_back("No differences found.");
            }
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to get differences: " + std::string(e.what()));
        }

        return differences;
    }
}

#endif // DIFF_HPP
