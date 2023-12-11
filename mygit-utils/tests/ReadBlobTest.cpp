#include <gtest/gtest.h>

#include "ZlibHelpers.hpp"
#include "Crypto.hpp"

#include <sstream>
#include <string_view>
#include <filesystem>
#include <fstream>

TEST(ReadBlob, when_valid_compressed_data_and_hash_passed_then_initial_data_received)
{
    // SHA1_HASH "358093dba4810321b2080662df818ea5a0ec1ce1"
    const auto compressed_data = "x\x9CK\xCA\xC9OR02d(\xCFH,Q\xC8,V(-\xD0Q\xF0\xAD\xCC\xCEH\xCC\xCC\xC9\xB7\a\x00\x8Bk\t\xA1";

    std::stringstream input_ss;
    input_ss << compressed_data;

    std::stringstream output_ss;
    if (git::readBlob(input_ss, output_ss))
    {
        EXPECT_TRUE(false);
    }

    const std::string uncompressed_data = output_ss.str();
    const std::string_view uncompressed_data_view { uncompressed_data };

    const auto null_pos = uncompressed_data.find_first_of('\0');
    ASSERT_NE(null_pos, std::string::npos);

    const std::string_view body = uncompressed_data_view.substr(null_pos + 1);
    const std::string_view expected_uncompressed_data = "what is up, Mykhailo?";
    ASSERT_STREQ(body.data(), expected_uncompressed_data.data());
}

TEST(ReadBlob, when_valid_test_blob_file_passed_and_then_initial_data_received)
{
    const std::filesystem::path filepath= "../../mygit-utils/tests/GitTestBlob";
    ASSERT_TRUE(std::filesystem::exists(filepath));

    std::ifstream input_file(filepath, std::ios::binary);
    ASSERT_TRUE(input_file.is_open());

    std::stringstream output_ss;
    if (git::readBlob(input_file, output_ss))
    {
        EXPECT_TRUE(false);
    }

    const std::string uncompressed_data = output_ss.str();
    const std::string_view uncompressed_data_view { uncompressed_data };

    const auto null_pos = uncompressed_data.find_first_of('\0');
    ASSERT_NE(null_pos, std::string::npos);

    const std::string_view body = uncompressed_data_view.substr(null_pos + 1);
    const std::string_view expected_uncompressed_data = "what is up, Mykhailo?";
    ASSERT_STREQ(body.data(), expected_uncompressed_data.data());
}

TEST(CalculateSHA, when_data_passed_to_calculate_hash_then_expected_hash_returned)
{
    const auto expected_sha1_hash = "f3da30b49e07349497a1707f303535fb74d4e070";
    const auto data = "what is up, Mykhailo?";

    const auto sha1_hash = git::calculateSha1Hash(data);
    ASSERT_EQ(sha1_hash, expected_sha1_hash);
}
