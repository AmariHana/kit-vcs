#ifndef MERGE_HPP
#define MERGE_HPP

#include "../utils/kit_utils.hpp"
#include "../utils/hash_object.hpp"
#include "../utils/error_handler.hpp"
#include "../utils/logger.hpp"
#include "../utils/constants.hpp"
#include "../utils/mock_kit_utils.hpp" // Include the definition of MockKitUtils

#include <string>
#include <filesystem>
#include <unordered_map>
#include <fstream>
#include <sstream>

namespace kit_vcs
{
    // Update the HEAD file with the given commit hash
    inline void update_head(const std::string &commit_hash)
    {
        try
        {
            kit_utils::create_file(HEAD_FILE, commit_hash);
            logger::info("HEAD updated to: " + commit_hash);
        }
        catch (const std::exception &e)
        {
            logger::error("Failed to update HEAD: " + std::string(e.what()));
            throw;
        }
    }

    // Retrieve files from a specific commit
    inline std::unordered_map<std::string, std::string> get_commit_files(const std::string &commit_hash)
    {
        std::unordered_map<std::string, std::string> files;
        std::string commit_path = OBJECTS_DIR + "/" + commit_hash;

        if (!std::filesystem::exists(commit_path))
        {
            throw std::runtime_error("Commit not found: " + commit_hash);
        }

        for (const auto &entry : std::filesystem::directory_iterator(commit_path))
        {
            if (entry.is_regular_file())
            {
                files[entry.path().filename().string()] = kit_utils::read_file(entry.path().string());
            }
        }

        return files;
    }

    // Merge a branch into the current branch
    inline bool merge_branch(const std::string &branch_name, MockKitUtils &kit_utils)
    {
        if (!kit_utils.ensure_repository_initialized())
        {
            logger::error("Repository is not initialized.");
            return false;
        }

        std::string branch_path = HEADS_DIR + "/" + branch_name;
        if (!std::filesystem::exists(branch_path))
        {
            logger::error("Branch does not exist: " + branch_name);
            return false;
        }

        try
        {
            // Read the current and target commit hashes
            std::string current_commit = kit_utils.read_file(HEAD_FILE);
            std::string target_commit = kit_utils.read_file(branch_path);

            if (current_commit == target_commit)
            {
                logger::info("Branches are already up to date.");
                return true;
            }

            // Find the common ancestor
            std::string base_commit = kit_utils.find_common_ancestor(current_commit, target_commit);
            if (base_commit.empty())
            {
                logger::error("No common ancestor found. Cannot merge.");
                return false;
            }

            // Perform a three-way merge
            auto merged_files = kit_utils.perform_three_way_merge(base_commit, current_commit, target_commit);

            // Create a new commit for the merge
            std::string new_commit_hash = kit_utils.create_commit(merged_files, "Merged branch " + branch_name);
            if (new_commit_hash.empty())
            {
                logger::error("Failed to create merge commit.");
                return false;
            }

            // Update HEAD to the new commit
            update_head(new_commit_hash);
            logger::info("Merge completed successfully.");
            return true;
        }
        catch (const std::exception &e)
        {
            logger::error("Failed to merge branches: " + std::string(e.what()));
            return false;
        }
    }
} // namespace kit_vcs

#endif // MERGE_HPP
