#include "InitCommand.hpp"
#include "Helpers.hpp"
#include "CommonVariables.hpp"
#include "HighLevelCommandFactory.hpp"

#include <fstream>
#include <cstring>

#include <argparse/argparse.hpp>

namespace git
{

class InitCommandFactory : public CommandFactory
{
    public:
        InitCommandFactory()
        {
            HighLevelCommandFactory::getInstance().addFactory(this);
        }

        std::unique_ptr<GitCommand>
        createCommand(const int argc, char** argv) override
        {
            if (std::strcmp(argv[1], "init") == 0)
            {
                argparse::ArgumentParser program("mygit_cli");
                program.add_argument("init");
                program.parse_args(argc, argv);

                return std::make_unique<InitCommand>();
            }

            return nullptr;
        }
};

InitCommandFactory init_factory;

void
InitCommand::execute()
{
    try
    {
        checkInitialized();
    }
    catch (const std::exception& e)
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
            throw std::runtime_error("InitCommand::execute - Failed to create .git/HEAD file.");
        }
    }
}

} // namespace git
