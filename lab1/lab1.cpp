// lab1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Number.h"
#include "Vector.h"

int main()
{
    std::cout << "Hello World!\n";
	Number n = NUMBER_ONE;

	Vector v(Number(3), Number(4));

	std::cout << (n / Number(2)).get_num() << " " << v.polar_phi().get_num() << "\n";

	std::cout << testvar << "\n";
	setval(1);
	std::cout << testvar << "\n";


	std::cin.get();
	std::cout << "\n";
	return 0;
}
