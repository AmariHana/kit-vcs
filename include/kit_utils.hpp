#ifndef KIT_UTILS_HPP
#define KIT_UTILS_HPP

#include <string>
#include <iostream>

// Forward declaration of the `kit_vcs` namespace and its function
namespace kit_vcs
{
    bool is_repository_initialized();
}

namespace kit_utils
{

    // Utility function to print colored messages
    inline void print_message(const std::string &message)
    {
        std::cout << "\033[1;32m[kit]\033[0m " << message << std::endl;
    }

    // Utility function to print error messages
    inline void print_error(const std::string &message)
    {
        std::cerr << "\033[1;31m[kit]\033[0m Error: " << message << std::endl;
    }

    // Ensure the repository is initialized
    inline bool ensure_repository_initialized()
    {
        if (!kit_vcs::is_repository_initialized())
        {
            print_error("No kit repository found. Please initialize a repository first.");
            return false;
        }
        return true;
    }

} // namespace kit_utils

#endif // KIT_UTILS_HPP
