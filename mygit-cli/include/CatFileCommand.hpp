#pragma once

#include "GitCommand.hpp"

#include <string>

namespace git
{

struct CatFileCommand : public GitCommand
{
    CatFileCommand(std::ostream& output) :
        m_output{output}
    {
    }

    void
    execute() override;

    std::string m_hash;
    std::ostream& m_output;
};

} // namespace git
