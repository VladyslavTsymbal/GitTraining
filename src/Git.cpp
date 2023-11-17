#include <iostream>
#include <string_view>

#include "InitCommand.hpp"
#include "CatFileCommand.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "No command provided.\n";
        exit(1);
    }

    const std::string_view command { argv[1] };
    if (command == "init")
    {
        git::InitCommand().execute();
    }
    else if (command == "cat-file")
    {
        if (argc < 4 or std::string_view(argv[2]) not_eq "-p")
        {
            std::cerr << "Invalid use of \'cat-file\'.\n";
            exit(1);
        }
        git::CatFileCommand().execute(argv[3], std::cout);
    }
    else
    {
        std::cerr << "Unknown git command.\n";
    }

    return EXIT_SUCCESS;
}