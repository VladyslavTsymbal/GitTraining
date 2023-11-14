#include "Git.hpp"

#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cassert>

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
        throw std::runtime_error(file.string() + " not exists.");
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

int
readBlob(std::istream& input, std::ostream& output)
{
    constexpr auto CHUNK_SIZE = 16384;

    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK_SIZE];
    unsigned char out[CHUNK_SIZE];

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);

    if (ret != Z_OK)
    {
        return ret;
    }

    do
    {
        if (input.read((char*)in, CHUNK_SIZE))
        {
            inflateEnd(&strm);
            return Z_ERRNO;
        }

        strm.avail_in = input.gcount();
        if (strm.avail_in == 0)
        {
            break;
        }
        strm.next_in = in;

        do
        {
            strm.avail_out = CHUNK_SIZE;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);

            switch (ret)
            {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR;
                    [[fallthrough]];
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    inflateEnd(&strm);
                    return ret;
            }

            have = CHUNK_SIZE - strm.avail_out;
            output.write((const char*)out, have);
        } while (strm.avail_out == 0);
   } while (ret != Z_STREAM_END);

   return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
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
        catFile(argv[3], std::cout);
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
Git::catFile(const std::string_view hash, std::ostream& output)
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

    std::ifstream source(name_path, std::ios::binary);
    if (not source.is_open())
    {
        throw std::runtime_error("Unable to open file: " + name_path.string());
    }

    std::stringstream ss;
    const int ret = readBlob(source, ss);
    if (ret)
    {
        throw std::runtime_error("Failed to readBlob: " + std::to_string(ret));
    }

    const auto data = ss.str();
    const std::string_view data_view { data };

    // Find end of header
    const auto pos = data_view.find_first_of('\0');
    if (pos == std::string::npos)
    {
        throw std::runtime_error("Probably invalid data, zero terminator for header found.");
    }

    const std::string_view body = data_view.substr(pos);
    output.write(&body[0], body.size());
}

} // git
