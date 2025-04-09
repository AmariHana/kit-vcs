cmake_minimum_required(VERSION 3.14)
project(kit-vcs VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(kit-vcs src/main.cpp)

# cxxopts
include(FetchContent)
FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG        v3.1.1
)
FetchContent_MakeAvailable(cxxopts)
target_link_libraries(kit-vcs PRIVATE cxxopts::cxxopts)
