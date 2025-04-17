#include <filesystem>
#include "../include/utils/hash_object.hpp"

namespace hash_object
{
    void ensure_kit_directory_exists()
    {
        if (!std::filesystem::exists(".kit"))
        {
            throw std::runtime_error("The .kit directory does not exist. Please initialize the repository.");
        }
    }
}
