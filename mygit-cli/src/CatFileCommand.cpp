#include "CatFileCommand.hpp"
#include "CommonVariables.hpp"
#include "Helpers.hpp"
#include "ZlibHelpers.hpp"
#include "HighLevelCommandFactory.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string_view>
#include <memory>
#include <cstring>

#include <argparse/argparse.hpp>

namespace git
{

class CatFileCommandFactory : public CommandFactory
{
    public:
        CatFileCommandFactory()
        {
            HighLevelCommandFactory::getInstance().addFactory(this);
        }

        std::unique_ptr<GitCommand>
        createCommand(const int argc, char** argv) override
        {
            if (std::strcmp(argv[1], "cat-file") == 0)
            {
                argparse::ArgumentParser program("mygit_cli");
                program.add_argument("cat-file");
                std::string hash;
                program.add_argument("-p").
                    store_into(hash).
                    help("Pretty-print the contents of <object> based on its type.");
                program.parse_args(argc, argv);

                auto command = std::make_unique<CatFileCommand>(std::cout);
                command->m_hash = std::move(hash);

                return command;
            }

            return nullptr;
        }
};

CatFileCommandFactory catfile_factory;

void
CatFileCommand::execute()
{
    try
    {
        checkInitialized();
    }
    catch (const std::exception& e)
    {
        throw;
    }

    if (m_hash.size() not_eq SHA1_STRING_LENGTH)
    {
        throw std::invalid_argument("CatFileCommand::execute - SHA1 hash isn't valid.");
    }

    std::filesystem::path directory_path;
    directory_path.append(GIT_OBJECTS_DIR);
    directory_path.append(m_hash.substr(0, 2));

    std::filesystem::path name_path;
    name_path.append(directory_path.string());
    name_path.append(m_hash.substr(2, 39));

    try
    {
        checkExists(directory_path);
        checkExists(name_path);
    }
    catch (const std::exception& e)
    {
        const std::string exception_message = std::string("CatFileCommand::execute - Hash to not existing object. ") + e.what();
        throw std::invalid_argument(exception_message);
    }

    std::ifstream source(name_path, std::ios::binary);
    if (not source.is_open())
    {
        throw std::runtime_error("CatFileCommand::execute - Unable to open file: " + name_path.string());
    }

    std::stringstream ss;
    const int ret = readBlob(source, ss);
    if (ret)
    {
        throw std::runtime_error("CatFileCommand::execute - Failed to readBlob: " + std::to_string(ret));
    }

    const auto data = ss.str();
    const std::string_view data_view { data };

    // Find end of header
    const auto pos = data_view.find_first_of('\0');
    if (pos == std::string::npos)
    {
        throw std::runtime_error("CatFileCommand::execute - Probably invalid data, zero terminator for header found.");
    }

    const std::string_view body = data_view.substr(pos);
    m_output.write(&body[0], body.size());
}

} // namespace git