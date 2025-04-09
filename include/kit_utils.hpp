#ifndef KIT_UTILS_HPP
#define KIT_UTILS_HPP

#include <string>
#include <iostream>

// Utility function to print colored messages
inline void print_message(const std::string& message) {
    std::cout << "\033[1;32m[kit]\033[0m " << message << std::endl;
}

#endif // KIT_UTILS_HPP
