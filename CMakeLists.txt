cmake_minimum_required(VERSION 3.13)

option(ENABLE_TESTS "Enable GTest tests" OFF)

project(git-starter-cpp)
set(CMAKE_CXX_STANDARD 20) # Enable the C++20 standard

set(GIT_SRCS src/Git.cpp
             src/InitCommand.cpp
             src/CatFileCommand.cpp
             src/utils/Helpers.cpp
             src/utils/ZlibHelpers.cpp)

set(GIT_INCLUDES include include/utils)

include(cmake/FetchDependencies.cmake)

add_executable(mygit ${GIT_SRCS})
target_compile_options(mygit PRIVATE -Wall -Wextra -Wpedantic -Werror)

# DEPENDENCIES_INCLUDES and DEPENDENCIES_LIBS are set in FetchDependencies.cmake
target_include_directories(mygit PRIVATE ${DEPENDENCIES_INCLUDES} ${GIT_INCLUDES})
target_link_libraries(mygit ${DEPENDENCIES_LIBS})