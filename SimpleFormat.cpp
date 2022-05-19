#include "SimpleFormat.hpp"

namespace format
{
    namespace internal
    {
        StringArgs::StringArgs(std::vector<std::string> args, std::size_t totalByteSize) :
            _args(std::move(args)),
            _totalByteSize(totalByteSize)
        {
        }

        std::size_t StringArgs::getTotalByteSize() const
        {
            return _totalByteSize;
        }

        const std::vector<std::string>& StringArgs::toVector() const
        {
            return _args;
        }
    }
}