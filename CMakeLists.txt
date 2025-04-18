# Minimum CMake version required
cmake_minimum_required(VERSION 3.14)

# Project details
project(kit-vcs VERSION 1.0.0 DESCRIPTION "A minimal version control system" LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable warnings for better code quality
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4)
endif()

# Set OpenSSL paths explicitly (for macOS/Homebrew)
set(OPENSSL_ROOT_DIR /opt/homebrew/opt/openssl@3)
set(OPENSSL_LIBRARIES /opt/homebrew/opt/openssl@3/lib)
set(OPENSSL_INCLUDE_DIR /opt/homebrew/opt/openssl@3/include)

# Find and link OpenSSL
find_package(OpenSSL REQUIRED)
if (NOT OpenSSL_FOUND)
    message(FATAL_ERROR "OpenSSL not found. Please install OpenSSL.")
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include ${OPENSSL_INCLUDE_DIR})

# Fetch and include external libraries
include(FetchContent)

# Fetch cxxopts library
FetchContent_Declare(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG        v3.1.1
)
FetchContent_MakeAvailable(cxxopts)

# Fetch Google Test library
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/**/*.hpp")

# Add the main executable
add_executable(kit-vcs ${SOURCES} ${HEADERS})
target_link_libraries(kit-vcs PRIVATE OpenSSL::SSL OpenSSL::Crypto cxxopts::cxxopts)

# Add unit tests
enable_testing()

# Test executable
file(GLOB TEST_SOURCES "tests/*.cpp")
add_executable(test_kit_vcs ${TEST_SOURCES})
target_link_libraries(test_kit_vcs PRIVATE OpenSSL::SSL OpenSSL::Crypto cxxopts::cxxopts gmock gtest)

# Add tests
add_test(NAME KitUtilsTest COMMAND test_kit_vcs)

# Output build details
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
