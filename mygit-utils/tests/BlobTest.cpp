#include <gtest/gtest.h>

#include "Blob.hpp"

#include <sstream>
#include <iostream>

TEST(BlobTest, when_valid_data_passed_then_blob_has_correct_fields)
{
    std::stringstream ss;
    ss << "blob 11\0";
    ss << " Four Five";

    git::Blob test_blob = git::Blob(ss);
    std::cout << test_blob.getObjectType() << std::endl;
    std::cout << test_blob.getObjectSize() << std::endl;
    std::cout << test_blob.getObjectBody() << std::endl;
}