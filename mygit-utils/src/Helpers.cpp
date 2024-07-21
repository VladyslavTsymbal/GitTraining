#include "Helpers.hpp"
#include "CommonVariables.hpp"

#include <stdexcept>
#include <vector>
#include <fstream>

namespace git
{

void
checkInitialized()
{
    checkExists(GIT_ROOT_DIR);
    checkExists(GIT_OBJECTS_DIR);
    checkExists(GIT_REFS_DIR);
    checkExists(GIT_HEAD_FILE);
}

void
checkExists(const std::filesystem::path file)
{
    const auto exists = std::filesystem::exists(file);
    if (not exists)
    {
        throw std::runtime_error("checkExists - " + file.string() + " not exists.");
    }
}

std::string
readBinaryFile(const std::filesystem::path filepath)
{
    checkExists(filepath);

    std::ifstream input_file(filepath, std::ios::binary);
    if (not input_file.is_open())
    {
        throw std::runtime_error(filepath.string() + ": failed to open.");
    }

    std::vector<char> file_content((std::istreambuf_iterator<char>(input_file)),
                               std::istreambuf_iterator<char>());
    std::string file_content_str(std::begin(file_content), std::end(file_content));

    return file_content_str;
}

} // namespace git
