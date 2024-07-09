#include "Crypto.hpp"

#include <sstream>
#include <iomanip>

#include <openssl/sha.h>

namespace git
{

// https://stackoverflow.com/a/16125797
std::string stringToHex(std::string_view data)
{
    std::stringstream ss;

    ss << std::hex << std::setfill('0');
    for (size_t i = 0; data.length() > i; ++i)
    {
        ss << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(data[i]));
    }

    return ss.str();
}

// https://stackoverflow.com/a/16125797
std::string hexToString(std::string_view data)
{
    std::string output;

    if ((data.length() % 2) not_eq 0)
    {
        throw std::invalid_argument("hexToString - string is not valid length.");
    }

    const auto cnt = data.length() / 2;

    for (size_t i = 0; cnt > i; ++i)
    {
        uint32_t s = 0;
        std::stringstream ss;
        ss << std::hex << data.substr(i * 2, 2);
        ss >> s;

        output.push_back(static_cast<unsigned char>(s));
    }

    return output;
}

std::string
calculateSha1Hash(std::string_view data)
{
    std::string sha1_hash;
    sha1_hash.resize(20);
    SHA1((const unsigned char*)data.begin(), data.size(), (unsigned char *)sha1_hash.data());
    return stringToHex(sha1_hash);
}

} // namespace git
