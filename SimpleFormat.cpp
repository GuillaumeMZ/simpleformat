#include <cassert>
#include <stdexcept>
#include "SimpleFormat.hpp"

namespace format::internal
{
    void overwrite_range(std::string& to, const std::string& from, std::size_t startTo, std::size_t startFrom, std::size_t endFrom) //TODO: move it to SimpleFormat.hpp
    {
        for(auto i = startFrom; i < endFrom; ++i)
            to[startTo + i - startFrom] = from[i];
    }
} //end of namespace format::internal