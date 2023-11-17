#pragma once

#include <string_view>

namespace git
{

constexpr std::string_view GIT_ROOT_DIR = ".git";
constexpr std::string_view GIT_OBJECTS_DIR = ".git/objects";
constexpr std::string_view GIT_REFS_DIR = ".git/refs";
constexpr std::string_view GIT_HEAD_FILE = ".git/HEAD";
constexpr auto SHA1_STRING_LENGTH = 40u;

} // namespace git
