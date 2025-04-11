#ifndef HASH_OBJECT_HPP
#define HASH_OBJECT_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>

namespace hash_object
{

    // Function to compute the SHA-1 hash of a given input string
    inline std::string compute_sha1(const std::string &content)
    {
        unsigned char hash[SHA_DIGEST_LENGTH];
        SHA1(reinterpret_cast<const unsigned char *>(content.c_str()), content.size(), hash);

        std::ostringstream oss;
        for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
        {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
        }
        return oss.str();
    }

} // namespace hash_object

#endif // HASH_OBJECT_HPP
