#include <iostream>
#include <cassert>
#include <filesystem>
#include <fstream>
#include "../include/kit_vcs.hpp"

void cleanup_test_environment()
{
    if (std::filesystem::exists(".kit"))
    {
        std::filesystem::remove_all(".kit");
    }
    std::cout << "[INFO] Cleaned up test environment.\n";
}

void test_initialize_repository()
{
    cleanup_test_environment();
    kit_vcs::initialize_repository();
    assert(std::filesystem::exists(".kit"));
    assert(std::filesystem::exists(".kit/HEAD"));
    assert(std::filesystem::exists(".kit/index"));
    assert(std::filesystem::exists(".kit/refs/heads"));
    assert(std::filesystem::exists(".kit/objects"));
    std::cout << "[PASS] test_initialize_repository passed.\n";
}

void test_branch_and_checkout()
{
    kit_vcs::create_branch("new-branch");
    assert(std::filesystem::exists(".kit/refs/heads/new-branch"));
    std::cout << "[PASS] Branch created successfully.\n";

    kit_vcs::change_branch("new-branch");
    std::ifstream head_file(".kit/HEAD");
    std::string head_content;
    std::getline(head_file, head_content);
    assert(head_content == "new-branch");
    std::cout << "[PASS] Branch checkout successful.\n";
}

void test_commit()
{
    std::ofstream file("file1.txt");
    file << "Hello, World!";
    file.close();
    kit_vcs::stage_file("file1.txt");

    bool commit_success = kit_vcs::create_commit("Initial commit");
    assert(commit_success);
    assert(std::filesystem::exists(".kit/objects"));
    std::cout << "[PASS] Commit created successfully.\n";
}

void test_diff()
{
    // Modify an existing file
    std::ofstream file("file1.txt", std::ios::app);
    if (!file)
    {
        throw std::runtime_error("Failed to open file1.txt for modification.");
    }
    file << "Modified content.";
    file.close();

    // Retrieve differences
    auto differences = kit_vcs::get_differences("HEAD");

    // Validate that differences are detected
    if (differences.empty())
    {
        throw std::runtime_error("No differences detected, but changes were made to file1.txt.");
    }

    // Print detected differences for debugging
    std::cout << "[DEBUG] Detected differences:\n";
    for (const auto &diff : differences)
    {
        std::cout << "  - " << diff << std::endl;
    }

    // Ensure the test passes if differences are detected
    std::cout << "[PASS] Diff detected changes successfully.\n";
}

void test_merge()
{
    kit_vcs::create_branch("feature-branch");
    kit_vcs::change_branch("feature-branch");

    std::ofstream feature_file("feature.txt");
    feature_file << "Feature branch content.";
    feature_file.close();
    kit_vcs::stage_file("feature.txt");
    kit_vcs::create_commit("Feature branch commit");

    kit_vcs::change_branch("master");
    bool merge_success = kit_vcs::merge_branch("feature-branch");
    assert(merge_success);
    assert(std::filesystem::exists("feature.txt"));
    std::cout << "[PASS] Merge completed successfully.\n";
}

void test_reset()
{
    std::ofstream file("file2.txt");
    file << "Temporary content.";
    file.close();
    kit_vcs::stage_file("file2.txt");
    kit_vcs::create_commit("Temporary commit");

    std::string previous_commit = "HEAD~1";
    kit_vcs::reset_to_commit(previous_commit);
    assert(!std::filesystem::exists("file2.txt"));
    std::cout << "[PASS] Reset to previous commit successful.\n";
}

void test_stash()
{
    std::ofstream file("file3.txt");
    file << "Stash this content.";
    file.close();

    kit_vcs::stash_changes();
    assert(!std::filesystem::exists("file3.txt"));
    std::cout << "[PASS] Stash changes successful.\n";
}

void test_status()
{
    std::ofstream file("file4.txt");
    file << "Unstaged changes.";
    file.close();

    auto status = kit_vcs::get_repository_status();
    assert(!status.empty());
    std::cout << "[PASS] Status detected changes successfully.\n";
}

int main()
{
    try
    {
        test_initialize_repository();
        test_branch_and_checkout();
        test_commit();
        test_diff();
        test_merge();
        test_reset();
        test_stash();
        test_status();
        std::cout << "All tests passed successfully.\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "[FAIL] Test failed: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
