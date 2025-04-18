#ifndef COMMIT_HPP
#define COMMIT_HPP

#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include "../utils/constants.hpp"
#include "../utils/kit_utils.hpp"
#include "../utils/hash_object.hpp"

namespace kit_vcs
{
    // Create a new commit
    inline bool create_commit(const std::string &message)
    {
        // Ensure there are staged files
        if (!kit_utils::has_staged_files())
        {
            kit_utils::print_error("No files staged for commit.");
            return false;
        }

        try
        {
            // Prepare commit content
            std::ostringstream commit_content;
            commit_content << "Commit message: " << message << "\n";

            // Add staged files to the commit
            std::ifstream index_file(INDEX_FILE);
            if (!index_file)
            {
                kit_utils::print_error("Failed to read index file.");
                return false;
            }

            std::string line;
            while (std::getline(index_file, line))
            {
                commit_content << line << "\n";
            }

            // Generate a hash for the commit
            std::string commit_hash = hash_object::compute_sha1(commit_content.str());
            std::cout << "[DEBUG] Generated commit hash: " << commit_hash << "\n";

            // Write the commit to the objects directory
            std::string commit_path = OBJECTS_DIR + "/" + commit_hash;
            kit_utils::create_file(commit_path, commit_content.str());
            std::cout << "[DEBUG] Commit written to: " << commit_path << "\n";

            // Update HEAD to point to the new commit
            kit_utils::update_head(commit_hash);
            std::cout << "[DEBUG] HEAD updated to: " << commit_hash << "\n";

            // Clear the index file after committing
            kit_utils::create_file(INDEX_FILE);
            std::cout << "[DEBUG] Index file cleared.\n";

            kit_utils::print_message("Commit created successfully with message: " + message);
            return true;
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to create commit: " + std::string(e.what()));
            return false;
        }
    }

    // Get the commit history
    inline std::unordered_map<std::string, std::string> get_working_directory_files()
    {
        std::unordered_map<std::string, std::string> files;

        for (const auto &entry : std::filesystem::recursive_directory_iterator("."))
        {
            if (entry.is_regular_file() && entry.path().string().find(KIT_DIR) == std::string::npos)
            {
                std::ifstream file(entry.path());
                std::stringstream buffer;
                buffer << file.rdbuf();
                files[entry.path().filename().string()] = buffer.str();
            }
        }

        return files;
    }

} // namespace kit_vcs

#endif // COMMIT_HPP
