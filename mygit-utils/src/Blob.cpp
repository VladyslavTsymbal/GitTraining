#include "Blob.hpp"

namespace git
{

Blob::Blob(const std::string& data)
{
    (void)data;
}

Blob::Blob(std::stringstream& stream)
{
    stream >> type;
    stream >> size;
    stream.getline()
}

std::string_view
Blob::getObjectType() const
{
    return type;
}

std::string_view
Blob::getObjectBody() const
{
    return body;
}

size_t
Blob::getObjectSize() const
{
    return size;
}

} // namespace git