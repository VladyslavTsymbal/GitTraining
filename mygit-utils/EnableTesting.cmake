enable_testing ()

set (TEST_DIR tests)
set (TEST_SRCS ${TEST_DIR}/ReadBlobTest.cpp
               ${TEST_DIR}/BlobTest.cpp)

add_executable (
  mygit_utils_test
  ${TEST_SRCS}
)

target_include_directories (mygit_utils_test PRIVATE include)
target_link_libraries (
  mygit_utils_test
  GTest::gtest_main
  mygit_utils
)

include (GoogleTest)
gtest_discover_tests (mygit_utils_test)
