#pragma once

#include <string>
#include <vector>

namespace format
{
    namespace internal
    {
        /************************************************************************/
        /*                              StringArgs                              */
        /************************************************************************/

        class StringArgs
        {
            public:
                StringArgs(std::vector<std::string> args, std::size_t totalByteSize);
                std::size_t getTotalByteSize() const;
                const std::vector<std::string>& toVector() const;

            private:
                std::vector<std::string> _args;
                std::size_t _totalByteSize;
        };

        template<typename T, typename... Ts>
        void add_next_string(std::vector<std::string>& vec, std::size_t index, std::size_t& totalByteSize, const T& current, const Ts&... remaining)
        {
            vec[index] = std::to_string(current); //TODO change the way a value is converted to a string
            totalByteSize += vec[index].size();

            if constexpr(sizeof...(remaining) > 0)
                add_next_string(vec, index + 1, totalByteSize, remaining...);
        }

        template<typename... Ts>
        StringArgs make_stringargs(const Ts&... args)
        {
            std::vector<std::string> result{sizeof...(args)};
            std::size_t totalByteSize{};

            add_next_string(result, 0, totalByteSize, args...);

            return {std::move(result), totalByteSize};
        }

        /************************************************************************/
        /*                                Pattern                               */
        /************************************************************************/

        class Pattern
        {
            public:
                Pattern(std::string pattern);
                const std::string& getPattern() const;
                const std::vector<std::size_t>& getPlaceholders() const;

            private:
                std::string _pattern;
                std::vector<std::size_t> _placeholders;
        };
    }
}
