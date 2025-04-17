# Minimum CMake version required
cmake_minimum_required(VERSION 3.14)

# Project details
project(kit-vcs VERSION 1.0.0 DESCRIPTION "A minimal version control system" LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output build details
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add the main executable
add_executable(kit-vcs
    src/main.cpp
    src/hash_object.cpp
)

# Include and link cxxopts library
include(FetchContent)
FetchContent_Declare(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG        v3.1.1
)
FetchContent_MakeAvailable(cxxopts)
target_link_libraries(kit-vcs PRIVATE cxxopts::cxxopts)

# Set OpenSSL paths explicitly
set(OPENSSL_ROOT_DIR /opt/homebrew/opt/openssl@3)
set(OPENSSL_LIBRARIES /opt/homebrew/opt/openssl@3/lib)
set(OPENSSL_INCLUDE_DIR /opt/homebrew/opt/openssl@3/include)

# Find and link OpenSSL
find_package(OpenSSL REQUIRED)
if (OpenSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
    target_link_libraries(kit-vcs PRIVATE OpenSSL::SSL OpenSSL::Crypto)
else()
    message(FATAL_ERROR "OpenSSL not found. Please install OpenSSL.")
endif()

# Enable warnings for better code quality
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(kit-vcs PRIVATE -Wall -Wextra -Wpedantic)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(kit-vcs PRIVATE /W4)
endif()

# Add include directories for the main executable
target_include_directories(kit-vcs PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Add unit tests
add_executable(test_kit_vcs
    tests/test_kit_vcs.cpp
)

# Link libraries for the test executable
target_link_libraries(test_kit_vcs PRIVATE
    cxxopts::cxxopts
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Add include directories for the test executable
target_include_directories(test_kit_vcs PRIVATE ${CMAKE_SOURCE_DIR}/include)
