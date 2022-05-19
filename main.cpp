#include "SimpleFormat.hpp"

//TODO:
// - switch from cmake to xmake
// - write tests
// - check if we can use forwarding references
// - better error handling & assertions
// - better naming conventions
// - document format::format and format::print
// - benchmarks !

int main()
{
    format::print("$$ + $ equals $", 123, 4, 4567, 1234 + 4567);
}
