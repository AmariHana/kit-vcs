#include <openssl/sha.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <iostream>
#include "error_handler.hpp"
#include "kit_utils.hpp" // Include kit_utils for print_message

namespace hash_object
{

    // Function to compute the SHA-1 hash of a given input string
    std::string compute_sha1(const std::string &content)
    {
        if (content.empty())
        {
            throw error_handler::KitException("Cannot compute SHA-1 hash of an empty string.");
        }

        unsigned char hash[SHA_DIGEST_LENGTH];
        SHA1(reinterpret_cast<const unsigned char *>(content.c_str()), content.size(), hash);

        std::ostringstream oss;
        for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
        {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
        }
        return oss.str();
    }

    // Function to hash a file object and store it in the .kit/objects directory
    void hash_object(const std::string &filename)
    {
        // Open the file in binary mode
        std::ifstream file(filename, std::ios::binary);
        if (!file)
        {
            error_handler::print_error("File not found or cannot be opened: " + filename);
            return;
        }

        // Read the file content into a string
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();

        // Create a Git-style blob header: "blob <size>\0<content>"
        std::string blob = "blob " + std::to_string(content.size()) + '\0' + content;

        // Compute the SHA-1 hash of the blob
        std::string hash = compute_sha1(blob);

        // Construct the directory and file paths for storing the object
        std::string dir = ".kit/objects/" + hash.substr(0, 2);
        std::string path = dir + "/" + hash.substr(2);

        // Create the directory if it doesn't exist
        try
        {
            std::filesystem::create_directories(dir);
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            error_handler::handle_exception(e, "Failed to create directory: " + dir);
            return;
        }

        // Write the blob to the file
        std::ofstream out(path, std::ios::binary);
        if (!out)
        {
            error_handler::print_error("Failed to write to file: " + path);
            return;
        }
        out << blob;
        out.close();

        // Output the hash to the console
        kit_utils::print_message("Object hashed successfully: " + hash); // Fixed here
    }

} // namespace hash_object
