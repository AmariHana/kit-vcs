# Minimum CMake version required
cmake_minimum_required(VERSION 3.14)

# Project details
project(kit-vcs VERSION 0.1.0 DESCRIPTION "A minimal version control system" LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output build details
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Enable warnings for better code quality
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4)
endif()

# Include and link cxxopts library
include(FetchContent)
FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG        v3.1.1
)
FetchContent_MakeAvailable(cxxopts)

# Find and link OpenSSL
find_package(OpenSSL REQUIRED)

# Add the main executable
add_executable(kit-vcs
    src/main.cpp
    src/hash_object.cpp
)

target_link_libraries(kit-vcs
    PRIVATE
    cxxopts::cxxopts
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Add the test executable
add_executable(test_kit_vcs
    tests/test_kit_vcs.cpp
)

target_link_libraries(test_kit_vcs
    PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
    cxxopts::cxxopts
)

# Add include directories for both executables
target_include_directories(kit-vcs PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(test_kit_vcs PRIVATE ${CMAKE_SOURCE_DIR}/include)
