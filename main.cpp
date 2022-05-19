#include <iostream>

#include "SimpleFormat.hpp"

int main()
{
    std::string a = "abcdefghijklmnopqrstuvwxyz";
    std::string b = "helloworld";

    format::internal::overwrite_range(a, b, 4, 3, 9);
    std::cout << a << std::endl;
}
