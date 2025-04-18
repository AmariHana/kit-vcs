#ifndef HASH_OBJECT_HPP
#define HASH_OBJECT_HPP

#include <string>
#include <sstream> // Include this header for std::ostringstream
#include <iomanip>
#include <openssl/sha.h>

namespace hash_object
{
    inline std::string compute_sha1(const std::string &input)
    {
        unsigned char hash[SHA_DIGEST_LENGTH];
        SHA1(reinterpret_cast<const unsigned char *>(input.c_str()), input.size(), hash);

        std::ostringstream oss;
        for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
        {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
        }
        return oss.str();
    }
}

#endif // HASH_OBJECT_HPP
