cmake_minimum_required(VERSION 3.13)
project(mygit-distribution)

message (STATUS "Make sure that you prefetched required dependencies!")
message (STATUS "Zlib is always required + GoogleTest if you want to enable test.")
message (STATUS "FetchContent should be used to add project's dependencies.")

add_subdirectory(mygit-utils)
