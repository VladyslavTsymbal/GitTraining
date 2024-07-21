#pragma once

#include "GitCommand.hpp"

#include <string>

namespace git
{

struct HashObjectCommand : public GitCommand
{
    void
    execute() override;

    std::string m_file_path;
};

} // namespace git