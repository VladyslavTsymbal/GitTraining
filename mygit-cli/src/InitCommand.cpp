#include "InitCommand.hpp"
#include "Helpers.hpp"
#include "CommonVariables.hpp"

#include <fstream>

namespace git
{

void
InitCommand::execute()
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

} // namespace git
