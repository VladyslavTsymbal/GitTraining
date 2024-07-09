#pragma once

#include <string_view>

namespace git
{

std::string
calculateSha1Hash(std::string_view data);

} // namespace git