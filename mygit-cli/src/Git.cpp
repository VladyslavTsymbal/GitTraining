#include <iostream>
#include <string_view>

#include "InitCommand.hpp"
#include "CatFileCommand.hpp"

#include <gflags/gflags.h>

int main(int argc, char* argv[])
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    if (argc < 2)
    {
        std::cerr << "No command provided.\n";
        exit(1);
    }

    const std::string_view command { argv[1] };

    try
    {
        if (command == "init")
        {
            git::InitCommand().execute();
        }
        else if (command == "cat-file")
        {
            git::CatFileCommand().execute(std::cout);
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