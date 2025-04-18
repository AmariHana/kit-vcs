#ifndef MOCK_KIT_UTILS_HPP
#define MOCK_KIT_UTILS_HPP

#include <gmock/gmock.h>
#include <string>
#include <unordered_map>

class MockKitUtils
{
public:
    // Mock method for ensuring the repository is initialized
    MOCK_METHOD(bool, ensure_repository_initialized, (), ());

    // Mock method for creating a file
    MOCK_METHOD(void, create_file, (const std::string &path, const std::string &content), ());

    // Mock method for reading a file
    MOCK_METHOD(std::string, read_file, (const std::string &path), ());

    // Mock method for finding the common ancestor of two commits
    MOCK_METHOD(std::string, find_common_ancestor, (const std::string &commit1, const std::string &commit2), ());

    // Mock method for performing a three-way merge
    MOCK_METHOD((std::unordered_map<std::string, std::string>), perform_three_way_merge,
                (const std::string &base_commit, const std::string &current_commit, const std::string &target_commit), ());

    // Mock method for creating a commit
    MOCK_METHOD(std::string, create_commit,
                (const std::unordered_map<std::string, std::string> &files, const std::string &message), ());
};

#endif // MOCK_KIT_UTILS_HPP
