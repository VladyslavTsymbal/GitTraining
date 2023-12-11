#pragma once

#include <string_view>
#include <istream>

namespace git
{

std::string
calculateSha1Hash(std::string_view data);

} // namespace git