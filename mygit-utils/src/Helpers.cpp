#include "Helpers.hpp"
#include "CommonVariables.hpp"

#include <stdexcept>

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
        throw std::runtime_error(file.string() + " not exists.");
    }
}

} // namespace git
