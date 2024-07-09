#pragma once

#include <memory>

namespace git
{

struct GitCommand;

class CommandFactory
{
    public:
        virtual
        ~CommandFactory() = default;

        virtual std::unique_ptr<GitCommand>
        createCommand(const int argc, char** argv) = 0;

    protected:
        CommandFactory() = default;
};

} // namespace git

