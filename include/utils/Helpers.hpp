#pragma once

#include <filesystem>

namespace git
{

void
checkInitialized();

void
checkExists(const std::filesystem::path file);

} // namespace git
