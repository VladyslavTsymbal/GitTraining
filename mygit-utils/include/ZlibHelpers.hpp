#pragma once

#include <istream>
#include <ostream>

namespace git
{

int
readBlob(std::istream& input, std::ostream& output);

int
writeBlob(std::istream& input, std::ostream& output, const int compression_level);

} // namespace git
