#ifndef COMMIT_HPP
#define COMMIT_HPP

#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "../utils/constants.hpp"
#include "../utils/kit_utils.hpp"
#include "../utils/hash_object.hpp"

namespace kit_vcs
{
    inline bool create_commit(const std::string &message)
    {
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
            index_file.close();

            // Generate a hash for the commit
            std::string commit_hash = hash_object::compute_sha1(commit_content.str());

            // Write the commit to the objects directory
            std::string commit_path = std::string(OBJECTS_DIR) + "/" + commit_hash;
            kit_utils::create_file(commit_path, commit_content.str());

            // Update HEAD to point to the new commit
            kit_utils::create_file(HEAD_FILE, commit_hash + "\n");

            // Clear the index file after committing
            kit_utils::create_file(INDEX_FILE);

            kit_utils::print_message("Commit created successfully with message: " + message);
            return true;
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to create commit: " + std::string(e.what()));
            return false;
        }
    }
} // namespace kit_vcs

#endif // COMMIT_HPP
