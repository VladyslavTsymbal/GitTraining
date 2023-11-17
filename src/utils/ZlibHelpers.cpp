#include "utils/ZlibHelpers.hpp"

#include <sstream>
#include <cassert>
#include <iostream>

#include <zlib.h>

namespace git
{

int
readBlob(std::istream& input, std::ostream& output)
{
    constexpr auto CHUNK_SIZE = 16384;

    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK_SIZE];
    unsigned char out[CHUNK_SIZE];

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);

    if (ret != Z_OK)
    {
        return ret;
    }

    do
    {
        if (input.read((char*)in, CHUNK_SIZE))
        {
            inflateEnd(&strm);
            return Z_ERRNO;
        }

        strm.avail_in = input.gcount();
        if (strm.avail_in == 0)
        {
            break;
        }
        strm.next_in = in;

        do
        {
            strm.avail_out = CHUNK_SIZE;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);

            switch (ret)
            {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR;
                    [[fallthrough]];
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    inflateEnd(&strm);
                    return ret;
            }

            have = CHUNK_SIZE - strm.avail_out;
            output.write((const char*)out, have);
        } while (strm.avail_out == 0);
   } while (ret != Z_STREAM_END);

   inflateEnd(&strm);
   return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

} // namespace git
