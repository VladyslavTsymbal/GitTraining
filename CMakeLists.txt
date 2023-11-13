cmake_minimum_required(VERSION 3.13)

include(FetchContent)
FetchContent_Declare(
    zlib
    GIT_REPOSITORY https://github.com/madler/zlib.git
    GIT_TAG        v1.3
)
FetchContent_MakeAvailable(zlib)
set(ZLIB_INCLUDE_DIR ${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})

project(git-starter-cpp)
set(CMAKE_CXX_STANDARD 20) # Enable the C++20 standard

set(SOURCE_FILES src/Server.cpp src/Git.cpp)

add_executable(server ${SOURCE_FILES})
target_compile_options(server PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_include_directories(server PRIVATE ${ZLIB_INCLUDE_DIR} include)
target_link_libraries(server zlib)