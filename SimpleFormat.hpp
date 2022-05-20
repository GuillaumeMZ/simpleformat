#pragma once

#include <array>
#include <cstdio>
#include <stdexcept>
#include <string>

namespace format
{
    namespace internal
    {
        /************************************************************************/
        /*                              StringArgs                              */
        /************************************************************************/

        template<std::size_t N>
        class StringArgs
        {
            public:
                StringArgs(std::array<std::string, N> args, std::size_t totalByteSize);
                std::size_t getTotalByteSize() const;
                const std::array<std::string, N>& toArray() const;

            private:
                std::array<std::string, N> _args;
                std::size_t _totalByteSize;
        };

        template<std::size_t N>
        StringArgs<N>::StringArgs(std::array<std::string, N> args, std::size_t totalByteSize) :
                _args(std::move(args)),
                _totalByteSize(totalByteSize)
        {
        }

        template<std::size_t N>
        std::size_t StringArgs<N>::getTotalByteSize() const
        {
            return _totalByteSize;
        }

        template<std::size_t N>
        const std::array<std::string, N>& StringArgs<N>::toArray() const
        {
            return _args;
        }

        template< std::size_t N, typename T, typename... Ts>
        void add_next_string(std::array<std::string, N>& vec, std::size_t index, std::size_t& totalByteSize, const T& current, const Ts&... remaining)
        {
            vec[index] = std::to_string(current); //TODO change the way a value is converted to a string
            totalByteSize += vec[index].size();

            if constexpr(sizeof...(remaining) > 0)
                add_next_string(vec, index + 1, totalByteSize, remaining...);
        }

        template<std::size_t N, typename... Ts>
        StringArgs<N> make_stringargs(const Ts&... args)
        {
            std::array<std::string, N> result{};
            std::size_t totalByteSize{};

            add_next_string(result, 0, totalByteSize, args...);

            return {std::move(result), totalByteSize};
        }

        /************************************************************************/
        /*                                Pattern                               */
        /************************************************************************/

        template<std::size_t N>
        class Pattern
        {
            public:
                Pattern(std::string pattern);
                const std::string& getPattern() const;
                const std::array<std::size_t, N>& getPlaceholders() const;

            private:
                std::string _pattern;
                std::array<std::size_t, N> _placeholders;
        };

        /************************************************************************/
        /*                                Pattern                               */
        /************************************************************************/

        template<std::size_t N>
        Pattern<N>::Pattern(std::string pattern) :
                _pattern(std::move(pattern))
        {
            for(auto i = 0, j = 0; i < _pattern.size(); ++i)
            {
                if(j >= N)
                    throw std::runtime_error("Too many placeholders");

                if(_pattern[i] == '$')
                {
                    _placeholders[j] = i;
                    ++j;
                }
            }
        }

        template<std::size_t N>
        const std::string& Pattern<N>::getPattern() const
        {
            return _pattern;
        }

        template<std::size_t N>
        const std::array<std::size_t, N>& Pattern<N>::getPlaceholders() const
        {
            return _placeholders;
        }

        void overwrite_range(std::string &to, const std::string &from, std::size_t startTo, std::size_t startFrom, std::size_t endFrom);

        template<std::size_t N>
        std::string fill_pattern(const Pattern<N>& pattern, const StringArgs<N>& args) //TODO: simplifier si possible
        {
            const auto& raw_pattern = pattern.getPattern();

            //Compute the total size of the string
            auto final_size = raw_pattern.size() - pattern.getPlaceholders().size() + args.getTotalByteSize();
            std::string result (final_size, '?'); //old way to initialize a string; bad >:(

            std::size_t globalOffset = 0;
            std::size_t lastNonReplacedIndex = 0;

            for(auto i = 0; i < pattern.getPlaceholders().size(); ++i)
            {
                const std::string& currentString = args.toArray()[i];
                const std::size_t currentPlaceholder = pattern.getPlaceholders()[i] + globalOffset - i;

                //Filling the non-placeholder part
                overwrite_range(result, raw_pattern, lastNonReplacedIndex, lastNonReplacedIndex - globalOffset + i, currentPlaceholder - globalOffset + i);
                lastNonReplacedIndex = currentPlaceholder + currentString.size();

                //Filling the placeholder part
                overwrite_range(result, currentString, currentPlaceholder, 0, currentString.size());
                globalOffset += currentString.size();
            }

            return result;
        }
    }

    template<typename... Ts>
    std::string format(std::string source, const Ts&... args)
    {
        constexpr auto N = sizeof...(Ts);

        if constexpr(N == 0)
            return source;

        internal::Pattern<N> pattern{std::move(source)};

        if(pattern.getPlaceholders().size() != N) //TODO: à retirer car ceci sera fait dans le pattern
            throw std::runtime_error("The number of arguments does not match the number of placeholders");

        const auto& stringArgs = internal::make_stringargs<N>(args...);

        return internal::fill_pattern(pattern, stringArgs);
    }

    template<typename... Ts>
    void print(const Ts&... args)
    {
        std::fputs(format(args...).c_str(), stdout); //because puts adds a newline whereas fputs doesn't
    }
}
