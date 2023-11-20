#pragma once

#include <string>
#include <string_view>
#include <sstream>

namespace git
{

class Blob
{
    public:
        Blob(const std::string& data);
        Blob(std::stringstream& stream);

        std::string_view
        getObjectType() const;

        std::string_view
        getObjectBody() const;

        size_t
        getObjectSize() const;

    private:
        std::string type;
        std::string body;
        size_t size;
};

} // namespace git
