#include <iostream>

#include "SimpleFormat.hpp"

int main()
{
    format::print("$ + $ equals $", 123, 4567, 123 + 4567);
}
