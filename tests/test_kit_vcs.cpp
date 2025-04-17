#include <iostream>
#include <cassert>
#include "../include/kit_vcs.hpp"

void test_initialize_repository()
{
    kit_vcs::initialize_repository();
    assert(std::filesystem::exists(".kit"));
    std::cout << "test_initialize_repository passed.\n";
}

int main()
{
    test_initialize_repository();
    std::cout << "All tests passed successfully.\n";
    return 0;
}
