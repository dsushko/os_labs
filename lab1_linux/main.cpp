#include <iostream>
#include "Number/Number.h"
#include "Vector/Vector.h"

int main()
{
    std::cout << "Hello World!\n";
    Number n = NUMBER_ONE;

    Vector v(Number(3), Number(4));

    std::cout << (n / Number(2)).get_num() << " " << v.polar_phi().get_num() << "\n";
    std::cin.get();
    return 0;
}