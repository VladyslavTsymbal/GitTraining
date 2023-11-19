cmake_minimum_required (VERSION 3.13)
project (mygit-cli)

include (FetchContent)

FetchContent_Declare (
    zlib
    GIT_REPOSITORY https://github.com/madler/zlib.git
    GIT_TAG        v1.3
)
FetchContent_MakeAvailable (zlib)

option (ENABLE_UTILS_TESTS "Enable MyGit-Utils tests" OFF)
if (ENABLE_UTILS_TESTS)
    FetchContent_Declare (
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
    )
    FetchContent_MakeAvailable (googletest)
    # Passthrough option to mygit_utils cmake
    # https://stackoverflow.com/a/54404924
    option (ENABLE_TESTS "Enable Utils tests" OFF)
    set (ENABLE_TESTS ON)
endif ()

set (MYGIT_UTILS_DIR ../mygit-utils)
set (MYGIT_UTILS_BUILD_DIR ../build/mygit-utils)
add_subdirectory (${MYGIT_UTILS_DIR} ${MYGIT_UTILS_BUILD_DIR})

# Enable the C++20 standard
set (CMAKE_CXX_STANDARD 20)

file (GLOB_RECURSE GIT_SRCS src/*.cpp)
add_executable (mygit_cli ${GIT_SRCS})
target_include_directories (mygit_cli PRIVATE include)
target_compile_options (mygit_cli PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries (mygit_cli PRIVATE mygit_utils)
