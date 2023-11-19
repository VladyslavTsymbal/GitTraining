cmake_minimum_required (VERSION 3.13)
project (mygit-utils)

# Enable the C++20 standard
set (CMAKE_CXX_STANDARD 20)

include (FetchContent)
FetchContent_GetProperties (zlib)
if (NOT zlib_POPULATED)
    message (FATAL_ERROR "Zlib is required to build mygit-utils static library. Use FetchContent to populate it.")
endif ()

option (ENABLE_TESTS "Enable Utils tests" OFF)
if (ENABLE_TESTS)
    FetchContent_GetProperties (googletest)
    if (NOT googletest_POPULATED)
        message (FATAL_ERROR "GoogleTest is required to build mygit-utils tests. Use FetchContent to populate it.")
    endif ()
    # Build tests
    include (EnableTesting.cmake)
endif ()

file (GLOB_RECURSE GIT_UTILS_SRCS src/*.cpp)
add_library (mygit_utils STATIC ${GIT_UTILS_SRCS})

target_include_directories(mygit_utils PUBLIC include)
set (ZLIB_INCLUDE_DIRS ${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})
target_include_directories(mygit_utils PRIVATE ${ZLIB_INCLUDE_DIRS})
target_compile_options (mygit_utils PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries (mygit_utils PRIVATE zlibstatic)
