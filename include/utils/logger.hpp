#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>

namespace logger
{
    inline void debug(const std::string &message)
    {
#ifdef DEBUG
        std::cout << "[DEBUG] " << message << std::endl;
#else
        (void)message; // Suppress unused parameter warning
#endif
    }

    inline void info(const std::string &message)
    {
        std::cout << "[INFO] " << message << std::endl;
    }

    inline void error(const std::string &message)
    {
        std::cerr << "[ERROR] " << message << std::endl;
    }
}

#endif // LOGGER_HPP
