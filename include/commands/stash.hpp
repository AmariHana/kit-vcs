#ifndef STASH_HPP
#define STASH_HPP

#include <string>
#include <fstream>
#include <filesystem>
#include "../utils/constants.hpp"
#include "../utils/kit_utils.hpp"

namespace kit_vcs
{
    inline bool stash_changes()
    {
        if (!kit_utils::ensure_repository_initialized())
        {
            return false;
        }

        try
        {
            std::string stash_path = std::string(KIT_DIR) + "/stash";
            std::ofstream stash_file(stash_path, std::ios::app);
            if (!stash_file)
            {
                kit_utils::print_error("Failed to open stash file.");
                return false;
            }

            // Add staged files to the stash
            std::ifstream index_file(INDEX_FILE);
            if (!index_file)
            {
                kit_utils::print_error("Failed to read index file.");
                return false;
            }

            std::string line;
            while (std::getline(index_file, line))
            {
                stash_file << line << "\n";
            }
            index_file.close();

            // Clear the index file after stashing
            kit_utils::create_file(INDEX_FILE);

            kit_utils::print_message("Changes stashed successfully.");
            return true;
        }
        catch (const std::exception &e)
        {
            kit_utils::print_error("Failed to stash changes: " + std::string(e.what()));
            return false;
        }
    }
} // namespace kit_vcs

#endif // STASH_HPP
