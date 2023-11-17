# This file is included from FetchDependencies.cmake in case ENABLE_TESTING=ON
# It's working this way to avoid using/linking gtest without previous fetch of the lib

enable_testing()

set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
# Fix later, no direct source files should be set here
set(SOURCES ${TEST_DIR}/ReadBlobTest.cpp src/utils/ZlibHelpers.cpp)

add_executable(
  git_test
  ${SOURCES}
)

target_include_directories(git_test PRIVATE ${GIT_INCLUDES})

target_link_libraries(
  git_test
  GTest::gtest_main
  # Fix later
  zlib
)

include(GoogleTest)
gtest_discover_tests(git_test)