#include <iostream>
#include <cassert>
#include "../include/kit_vcs.hpp"

void test_initialize_repository()
{
    kit_vcs::initialize_repository();
    assert(std::filesystem::exists(".kit"));
    assert(std::filesystem::exists(".kit/HEAD"));
    assert(std::filesystem::exists(".kit/index"));
    std::cout << "test_initialize_repository passed.\n";
}

void test_stage_file()
{
    std::ofstream test_file("test.txt");
    test_file << "Hello, kit-vcs!";
    test_file.close();

    bool result = kit_vcs::stage_file("test.txt");
    assert(result);
    std::cout << "test_stage_file passed.\n";
}

void test_commit()
{
    bool result = kit_vcs::create_commit("Initial commit");
    assert(result);
    std::cout << "test_commit passed.\n";
}

void test_branch_creation()
{
    bool result = kit_vcs::create_branch("new-branch");
    assert(result);
    std::cout << "test_branch_creation passed.\n";
}

void test_switch_branch()
{
    bool result = kit_vcs::switch_branch("new-branch");
    assert(result);
    std::cout << "test_switch_branch passed.\n";
}

int main()
{
    test_initialize_repository();
    test_stage_file();
    test_commit();
    test_branch_creation();
    test_switch_branch();

    std::cout << "All tests passed successfully.\n";
    return 0;
}
