cmake_minimum_required(VERSION 3.13)

include(FetchDependencies.cmake)

project(git-starter-cpp)
set(CMAKE_CXX_STANDARD 20) # Enable the C++20 standard

set(SOURCE_FILES src/Server.cpp src/Git.cpp)

add_executable(server ${SOURCE_FILES})
target_compile_options(server PRIVATE -Wall -Wextra -Wpedantic -Werror)

# DEPENDENCIES_INCLUDES and DEPENDENCIES_LIBS are set in FetchDependencies.cmake
target_include_directories(server PRIVATE ${DEPENDENCIES_INCLUDES} include)
target_link_libraries(server ${DEPENDENCIES_LIBS})