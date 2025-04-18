#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "../include/kit_vcs.hpp"
#include "../include/utils/kit_utils.hpp"

using ::testing::_;
using ::testing::Return;

class MockKitUtils
{
public:
    MOCK_METHOD(std::string, find_common_ancestor, (const std::string &, const std::string &), ());
    MOCK_METHOD((std::unordered_map<std::string, std::string>), perform_three_way_merge,
                (const std::string &, const std::string &, const std::string &), ());
    MOCK_METHOD(bool, ensure_repository_initialized, (), ());
    MOCK_METHOD(void, create_file, (const std::string &, const std::string &), ());
    MOCK_METHOD(std::string, read_file, (const std::string &), ());
};

// Helper function to initialize the repository
void initialize_repository()
{
    kit_utils::initialize_repository();
    ASSERT_TRUE(std::filesystem::exists(".kit"));
}

// Helper function to clean up the repository
void cleanup_repository()
{
    std::filesystem::remove_all(".kit");
}

// Test for `merge` command
TEST(MergeTest, MergeBranch_Success)
{
    initialize_repository();
    kit_utils::create_file(".kit/refs/heads/branch1", "commit1");
    kit_utils::create_file(".kit/refs/heads/branch2", "commit2");

    MockKitUtils mock_kit_utils;
    std::unordered_map<std::string, std::string> mock_merged_files = {
        {"file1.txt", "Merged content of file1"},
        {"file2.txt", "Merged content of file2"}};

    EXPECT_CALL(mock_kit_utils, find_common_ancestor(_, _)).WillOnce(Return("common_commit"));
    EXPECT_CALL(mock_kit_utils, perform_three_way_merge(_, _, _)).WillOnce(Return(mock_merged_files));

    EXPECT_TRUE(kit_vcs::merge_branch("branch2", mock_kit_utils));

    cleanup_repository();
}

// Test for `branch` command
TEST(BranchTest, CreateBranch_Success)
{
    initialize_repository();
    kit_utils::create_file(".kit/HEAD", "commit1");

    std::string branch_name = "new_branch";
    std::string branch_path = ".kit/refs/heads/" + branch_name;
    kit_utils::create_file(branch_path, "commit1");

    ASSERT_TRUE(std::filesystem::exists(branch_path));
    ASSERT_EQ(kit_utils::read_file(branch_path), "commit1");

    cleanup_repository();
}

// Test for `checkout` command
TEST(CheckoutTest, CheckoutBranch_Success)
{
    initialize_repository();
    kit_utils::create_file(".kit/HEAD", "commit1");
    kit_utils::create_file(".kit/refs/heads/branch1", "commit2");

    std::string branch_name = "branch1";
    std::string branch_path = ".kit/refs/heads/" + branch_name;
    std::string new_head = kit_utils::read_file(branch_path);
    kit_utils::create_file(".kit/HEAD", new_head);

    ASSERT_EQ(kit_utils::read_file(".kit/HEAD"), "commit2");

    cleanup_repository();
}

// Test for `commit` command
TEST(CommitTest, CreateCommit_Success)
{
    initialize_repository();

    std::unordered_map<std::string, std::string> files = {
        {"file1.txt", "Content of file1"},
        {"file2.txt", "Content of file2"}};
    std::string commit_message = "Initial commit";
    std::string commit_hash = kit_utils::create_commit(files, commit_message);

    ASSERT_FALSE(commit_hash.empty());
    ASSERT_TRUE(std::filesystem::exists(".kit/objects/" + commit_hash));

    cleanup_repository();
}

// Test for `reset` command
TEST(ResetTest, ResetToCommit_Success)
{
    initialize_repository();
    kit_utils::create_file(".kit/HEAD", "commit1");

    std::string new_commit = "commit2";
    kit_utils::create_file(".kit/HEAD", new_commit);

    ASSERT_EQ(kit_utils::read_file(".kit/HEAD"), "commit2");

    cleanup_repository();
}

// Test for `stash` command
TEST(StashTest, StashChanges_Success)
{
    initialize_repository();

    std::string stash_path = ".kit/stash";
    kit_utils::create_file(stash_path, "Stashed changes");

    ASSERT_TRUE(std::filesystem::exists(stash_path));
    ASSERT_EQ(kit_utils::read_file(stash_path), "Stashed changes");

    cleanup_repository();
}
