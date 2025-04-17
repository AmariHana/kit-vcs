#ifndef ERROR_HANDLER_HPP
#define ERROR_HANDLER_HPP

#include <iostream>
#include <stdexcept>
#include <string>

namespace error_handler
{

    // Function to print error messages
    inline void print_error(const std::string &message)
    {
        std::cerr << "\033[1;31m[kit]\033[0m Error: " << message << std::endl;
    }

    // Function to print warning messages
    inline void print_warning(const std::string &message)
    {
        std::cerr << "\033[1;33m[kit]\033[0m Warning: " << message << std::endl;
    }

    // Function to handle exceptions
    inline void handle_exception(const std::exception &e, const std::string &context = "")
    {
        if (!context.empty())
        {
            print_error(context + ": " + e.what());
        }
        else
        {
            print_error(e.what());
        }
    }

    // Custom exception class for kit-vcs
    class KitException : public std::runtime_error
    {
    public:
        explicit KitException(const std::string &message)
            : std::runtime_error(message) {}
    };

} // namespace error_handler

#endif // ERROR_HANDLER_HPP
