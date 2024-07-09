#pragma once

#include "GitCommand.hpp"

namespace git
{

struct InitCommand : public GitCommand
{
    void
    execute() override;
};

} // namespace git
