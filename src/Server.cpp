#include <iostream>
#include <string>

#include "Git.hpp"

int main(int argc, char* argv[])
{
    using git::Git;

    try
    {
        Git::processCommand(argc, argv);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return EXIT_SUCCESS;
}
