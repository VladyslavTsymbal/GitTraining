#include "HashObjectCommand.hpp"
#include "HighLevelCommandFactory.hpp"
#include "Crypto.hpp"
#include "Helpers.hpp"

#include <stdexcept>
#include <string>
#include <cstring>

#include <argparse/argparse.hpp>

// DEBUG
#include <iostream>

namespace git
{

class HashObjectCommandFactory : public CommandFactory
{
    public:
        HashObjectCommandFactory()
        {
            HighLevelCommandFactory::getInstance().addFactory(this);
        }

        std::unique_ptr<GitCommand>
        createCommand(const int argc, char** argv) override
        {
            if (std::strcmp(argv[1], "hash-object") == 0)
            {
                argparse::ArgumentParser program("mygit_cli");
                program.add_argument("hash-object");
                program.add_argument("-w").
                    help("Actually write the object into the object database.");
                auto &group = program.add_mutually_exclusive_group(true);
                group.add_argument("--stdin").
                    help("Read the object from standard input instead of from a file.").
                    flag();
                std::string file_path;
                group.add_argument("--path").
                    store_into(file_path).
                    help("Hash object located at the given path.");

                program.parse_args(argc, argv);

                auto command = std::make_unique<HashObjectCommand>();
                command->m_file_path = std::move(file_path);

                return command;
            }

            return nullptr;
        }
};

HashObjectCommandFactory hashobject_factory;

void
HashObjectCommand::execute()
{
    std::string hash;
    // if (FLAGS_stdin)
    // {
    //     std::cout << "FROM STDIN\n";
    // }
    // else
    // {
        std::cout << "filepath: " << m_file_path << std::endl;
        const std::string file_content = readBinaryFile(m_file_path);
        hash = calculateSha1Hash(file_content);
        std::cout << "HASH: " << hash << std::endl;
    // }

    // if (FLAGS_w)
    // {
    //     checkInitialized();


    // }
}


} // namespace git
