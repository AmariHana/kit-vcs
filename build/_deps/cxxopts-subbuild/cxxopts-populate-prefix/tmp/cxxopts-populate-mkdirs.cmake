# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/hana/Desktop/kit-vcs/build/_deps/cxxopts-src")
  file(MAKE_DIRECTORY "/Users/hana/Desktop/kit-vcs/build/_deps/cxxopts-src")
endif()
file(MAKE_DIRECTORY
  "/Users/hana/Desktop/kit-vcs/build/_deps/cxxopts-build"
  "/Users/hana/Desktop/kit-vcs/build/_deps/cxxopts-subbuild/cxxopts-populate-prefix"
  "/Users/hana/Desktop/kit-vcs/build/_deps/cxxopts-subbuild/cxxopts-populate-prefix/tmp"
  "/Users/hana/Desktop/kit-vcs/build/_deps/cxxopts-subbuild/cxxopts-populate-prefix/src/cxxopts-populate-stamp"
  "/Users/hana/Desktop/kit-vcs/build/_deps/cxxopts-subbuild/cxxopts-populate-prefix/src"
  "/Users/hana/Desktop/kit-vcs/build/_deps/cxxopts-subbuild/cxxopts-populate-prefix/src/cxxopts-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/hana/Desktop/kit-vcs/build/_deps/cxxopts-subbuild/cxxopts-populate-prefix/src/cxxopts-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/hana/Desktop/kit-vcs/build/_deps/cxxopts-subbuild/cxxopts-populate-prefix/src/cxxopts-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
