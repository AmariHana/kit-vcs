#ifndef HASH_OBJECT_HPP
#define HASH_OBJECT_HPP

#include <string>
#include <sstream> // Include this header for std::ostringstream
#include <iomanip>
#include <openssl/sha.h>

namespace hash_object
{
    inline std::string to_hex(const unsigned char *hash, size_t length)
    {
        std::ostringstream oss;
        for (size_t i = 0; i < length; ++i)
        {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
        }
        return oss.str();
    }

    inline std::string compute_sha1(const std::string &input)
    {
        unsigned char hash[SHA_DIGEST_LENGTH];
        SHA1(reinterpret_cast<const unsigned char *>(input.c_str()), input.size(), hash);
        return to_hex(hash, SHA_DIGEST_LENGTH);
    }
}

#endif // HASH_OBJECT_HPP
