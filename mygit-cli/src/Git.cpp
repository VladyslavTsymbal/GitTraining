#include "GitCommand.hpp"
#include "HighLevelCommandFactory.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        const auto command_ptr = git::HighLevelCommandFactory::getInstance().
            createCommand(argc, argv);
        if (command_ptr)
        {
            command_ptr->execute();
        }
        else
        {
            std::cerr << "Unknown git command.\n";
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return EXIT_SUCCESS;
}