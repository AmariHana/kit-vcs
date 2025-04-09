# Minimum CMake version required
cmake_minimum_required(VERSION 3.14)

# Project details
project(kit-vcs VERSION 0.1.0 DESCRIPTION "A minimal version control system" LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the main executable
add_executable(kit-vcs src/main.cpp src/hash_object.cpp)

# Include and link cxxopts library
include(FetchContent)
FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG        v3.1.1
)
FetchContent_MakeAvailable(cxxopts)
target_link_libraries(kit-vcs PRIVATE cxxopts::cxxopts)

# Find and link OpenSSL
find_package(OpenSSL REQUIRED)
target_link_libraries(kit-vcs PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Enable warnings for better code quality
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(kit-vcs PRIVATE -Wall -Wextra -Wpedantic)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(kit-vcs PRIVATE /W4)
endif()

# Add include directories
target_include_directories(kit-vcs PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Output build details
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
