#pragma once

#include <iostream>
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

        void overwrite_range(std::string &to, const std::string &from, std::size_t startTo, std::size_t startFrom, std::size_t endFrom);
        std::string fill_pattern(const Pattern &pattern, const StringArgs &args);
    }

    template<typename... Ts>
    std::string format(std::string source, const Ts&... args)
    {
        //Check if the number of arguments is 0
        if constexpr(sizeof...(args) == 0)
            return source;

        internal::Pattern pattern{std::move(source)};

        //Check if the number of arguments is equal to the number of placeholders
        if(pattern.getPlaceholders().size() != sizeof...(args))
            throw std::runtime_error("The number of arguments does not match the number of placeholders");

        //Create the StringArgs
        const auto& stringArgs = internal::make_stringargs(args...);

        //Fill the pattern
        return internal::fill_pattern(pattern, stringArgs);
    }

    template<typename... Ts>
    void print(const Ts&... args)
    {
        std::cout << format(args...);
    }
}
