#include "Git.hpp"

#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <algorithm>

#include <zlib.h>

namespace git
{

constexpr std::string_view GIT_ROOT_DIR = ".git";
constexpr std::string_view GIT_OBJECTS_DIR = ".git/objects";
constexpr std::string_view GIT_REFS_DIR = ".git/refs";
constexpr std::string_view GIT_HEAD_FILE = ".git/HEAD";
constexpr auto SHA1_STRING_LENGTH = 40u;

void
checkExists(const std::filesystem::path file)
{
    const auto exists = std::filesystem::exists(file);
    if (not exists)
    {
        const auto exception_message = file.string() + " not exists.";
        throw std::runtime_error(exception_message);
    }
}

void
checkInitialized()
{
    checkExists(GIT_ROOT_DIR);
    checkExists(GIT_OBJECTS_DIR);
    checkExists(GIT_REFS_DIR);
    checkExists(GIT_HEAD_FILE);
}

std::string
readBlob(const std::filesystem::path name_path)
{
    std::ifstream source(name_path, std::ios::binary);
    if (not source.is_open())
    {
        const auto exception_error = std::string("Unable to open file: ") + name_path.string();
        throw std::runtime_error(exception_error);
    }

    const auto compressed_data = std::string(std::istreambuf_iterator<char>(source),
                                             std::istreambuf_iterator<char>());

    uLongf uncompressed_data_size = compressed_data.size() * 10;
    std::string uncompressed_data(uncompressed_data_size, '\0');

    if (uncompress(reinterpret_cast<Bytef*>(&uncompressed_data[0]), &uncompressed_data_size,
                   reinterpret_cast<const Bytef*>(compressed_data.c_str()), compressed_data.size()) != Z_OK)
    {
        throw std::runtime_error("Failed to uncompress blob.");
    }

    const auto header_null_pos = uncompressed_data.find_first_of('\0');
    if (header_null_pos == std::string::npos)
    {
        std::cerr << "Unable to find header's null symbol\n";
    }

    // HACK
    std::string result = uncompressed_data.substr(header_null_pos + 1);
    const auto first_null_pos = result.find_first_of('\0');
    result.erase(first_null_pos);

    return result;
}

void
Git::processCommand(const int argc, char** argv)
{
    if (argc < 2)
    {
        throw std::invalid_argument("No command provided.");
    }

    const std::string_view command { argv[1] };
    if (command == "init")
    {
        init();
    }
    else if (command == "cat-file")
    {
        if (argc < 4 or std::string_view(argv[2]) not_eq "-p")
        {
            throw std::invalid_argument("Invalid use of \'cat-file\'.");
        }
        catFile(argv[3]);
    }
    else
    {
        throw std::invalid_argument("Unknown git command.");
    }
}

void
Git::init()
{
    try
    {
        checkInitialized();
    }
    catch(const std::exception& e)
    {
        std::filesystem::create_directory(GIT_ROOT_DIR);
        std::filesystem::create_directory(GIT_OBJECTS_DIR);
        std::filesystem::create_directory(GIT_REFS_DIR);

        std::string head(GIT_HEAD_FILE);
        std::ofstream headFile(head);
        if (headFile.is_open())
        {
            headFile << "ref: refs/heads/master\n";
            headFile.close();
        }
        else
        {
            throw std::runtime_error("Failed to create .git/HEAD file.");
        }
    }
}

void
Git::catFile(const std::string_view hash)
{
    try
    {
        checkInitialized();
    }
    catch(const std::exception& e)
    {
        throw;
    }

    if (hash.size() not_eq SHA1_STRING_LENGTH)
    {
        throw std::invalid_argument("SHA1 hash isn't valid.");
    }

    std::filesystem::path directory_path;
    directory_path.append(GIT_OBJECTS_DIR);
    directory_path.append(hash.substr(0, 2));

    std::filesystem::path name_path;
    name_path.append(directory_path.string());
    name_path.append(hash.substr(2, 39));

    try
    {
        checkExists(directory_path);
        checkExists(name_path);
    }
    catch(const std::exception& e)
    {
        const std::string exception_message = std::string("Hash to not existing object. ") + e.what();
        throw std::invalid_argument(exception_message);
    }

    try
    {
        const std::string blob_content = readBlob(name_path);
        std::cout << blob_content;
    }
    catch(const std::exception& e)
    {
        const auto exception_message = "Read blob failed.";
        throw std::runtime_error(exception_message);
    }
}

} // git