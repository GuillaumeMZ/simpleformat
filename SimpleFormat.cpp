#include <cassert>
#include <stdexcept>
#include "SimpleFormat.hpp"

namespace format::internal
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

    /************************************************************************/
    /*                                Pattern                               */
    /************************************************************************/

    Pattern::Pattern(std::string pattern) :
        _pattern(std::move(pattern))
    {
        for(auto i = 0; i < _pattern.size(); ++i)
            if(_pattern[i] == '$')
                _placeholders.push_back(i);
    }

    const std::string& Pattern::getPattern() const
    {
        return _pattern;
    }

    const std::vector<std::size_t>& Pattern::getPlaceholders() const
    {
        return _placeholders;
    }

    void overwrite_range(std::string& to, const std::string& from, std::size_t startTo, std::size_t startFrom, std::size_t endFrom)
    {
        for(auto i = startFrom; i < endFrom; ++i)
            to[startTo + i - startFrom] = from[i];
    }

    std::string fill_pattern(const Pattern& pattern, const StringArgs& args)
    {
        if(args.toVector().size() != pattern.getPlaceholders().size())
            throw std::runtime_error("The number of arguments does not match the number of placeholders");

        const auto& raw_pattern = pattern.getPattern();

        //Compute the total size of the string
        auto final_size = raw_pattern.size() - pattern.getPlaceholders().size() + args.getTotalByteSize();
        std::string result (final_size, '?'); //old way to initialize a string; bad >:(

        std::size_t globalOffset = 0;
        std::size_t lastNonReplacedIndex = 0;

        for(auto i = 0; i < pattern.getPlaceholders().size(); ++i)
        {
            const std::string& currentString = args.toVector()[i];
            const std::size_t currentPlaceholder = pattern.getPlaceholders()[i] + globalOffset - i;

            //Filling the non-replaced part
            overwrite_range(result, raw_pattern, lastNonReplacedIndex, lastNonReplacedIndex - globalOffset + i, currentPlaceholder - globalOffset + i);
            lastNonReplacedIndex = currentPlaceholder + currentString.size();

            //Filling the replaced part
            overwrite_range(result, currentString, currentPlaceholder, 0, currentString.size());
            globalOffset += currentString.size();
        }

        return result;
    }
} //end of namespace format::internal