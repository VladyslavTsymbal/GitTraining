#pragma once

#include <istream>
#include <ostream>

namespace git
{

int
readBlob(std::istream& input, std::ostream& output);

} // namespace git
