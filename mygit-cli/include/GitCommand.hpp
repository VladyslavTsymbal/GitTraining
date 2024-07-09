#pragma once

namespace git
{

struct GitCommand
{
    virtual
    ~GitCommand() = default;

    virtual void
    execute() = 0;
};

} // namespace git
