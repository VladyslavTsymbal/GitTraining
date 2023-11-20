#pragma once

#include <string_view>
#include <ostream>

namespace git
{

struct CatFileCommand
{
	void
	execute(std::ostream& output);
};

} // namespace git
