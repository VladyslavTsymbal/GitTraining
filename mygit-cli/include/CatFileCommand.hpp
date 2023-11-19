#pragma once

#include <string_view>
#include <ostream>

namespace git
{

struct CatFileCommand
{
	void
	execute(const std::string_view hash, std::ostream& output);
};

} // namespace git
