#include "CatFileCommand.hpp"
#include "CommonVariables.hpp"
#include "Helpers.hpp"
#include "ZlibHelpers.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <gflags/gflags.h>

DEFINE_string(p, "sha1 hash", "Pretty-print the contents of <object> based on its type.");

namespace git
{

void
CatFileCommand::execute(std::ostream& output)
{
    using namespace GFLAGS_NAMESPACE;

    std::string p_command_line_arg;
    const bool p_found = GetCommandLineOption("p", &p_command_line_arg);
    if (p_found)
    {
        const std::string_view hash { p_command_line_arg };
        try
        {
            checkInitialized();
        }
        catch (const std::exception& e)
        {
            throw;
        }

        if (hash.size() not_eq SHA1_STRING_LENGTH)
        {
            throw std::invalid_argument("SHA1 hash isn't valid.");
        }

        std::filesystem::path directory_path;
        directory_path.append(GIT_OBJECTS_DIR);
        directory_path.append(hash.substr(0, 2));

        std::filesystem::path name_path;
        name_path.append(directory_path.string());
        name_path.append(hash.substr(2, 39));

        try
        {
            checkExists(directory_path);
            checkExists(name_path);
        }
        catch (const std::exception& e)
        {
            const std::string exception_message = std::string("Hash to not existing object. ") + e.what();
            throw std::invalid_argument(exception_message);
        }

        std::ifstream source(name_path, std::ios::binary);
        if (not source.is_open())
        {
            throw std::runtime_error("Unable to open file: " + name_path.string());
        }

        std::stringstream ss;
        const int ret = readBlob(source, ss);
        if (ret)
        {
            throw std::runtime_error("Failed to readBlob: " + std::to_string(ret));
        }

        const auto data = ss.str();
        const std::string_view data_view { data };

        // Find end of header
        const auto pos = data_view.find_first_of('\0');
        if (pos == std::string::npos)
        {
            throw std::runtime_error("Probably invalid data, zero terminator for header found.");
        }

        const std::string_view body = data_view.substr(pos);
        output.write(&body[0], body.size());
    }
}

} // namespace git