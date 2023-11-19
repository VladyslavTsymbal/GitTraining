#include <gtest/gtest.h>

#include "ZlibHelpers.hpp"

#include <sstream>
#include <string_view>

TEST(ReadBlob, when_valid_compressed_data_and_hash_passed_then_initial_data_received)
{
    // SHA1_HASH "358093dba4810321b2080662df818ea5a0ec1ce1"
    const auto compressed_data = "x\x9CK\xCA\xC9OR02d(\xCFH,Q\xC8,V(-\xD0Q\xF0\xAD\xCC\xCEH\xCC\xCC\xC9\xB7\a\x00\x8Bk\t\xA1";
    const auto expected_uncompressed_data = "what is up, Mykhailo?";

    std::stringstream input_ss;
    input_ss << compressed_data;

    std::stringstream output_ss;
    if (git::readBlob(input_ss, output_ss))
    {
        // Investigate ReadBlob return value (should be 0 in success)
        // EXPECT_TRUE(false);
    }

    const std::string uncompressed_data = output_ss.str();
    const std::string_view uncompressed_data_view { uncompressed_data };

    const auto null_pos = uncompressed_data.find_first_of('\0');
    ASSERT_NE(null_pos, std::string::npos);

    const std::string_view body = uncompressed_data_view.substr(null_pos + 1);
    ASSERT_EQ(body, expected_uncompressed_data);
}