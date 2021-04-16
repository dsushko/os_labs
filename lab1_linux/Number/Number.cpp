// Number.cpp : Определяет функции для статической библиотеки.
//

#include "Number.h"

// TODO: Это пример библиотечной функции.

Number::Number(double num) {
		this->num = num;
	};
Number::Number() {
		num = 0;
	}
	Number Number::operator+(const Number& n) {
		return Number(num + n.num);
	}
	Number Number::operator-(const Number& n) {
		return Number(num - n.num);
	}
	Number Number::operator*(const Number& n) {
		return Number(num * n.num);
	}
	Number Number::operator/(const Number& n) {
		return Number(num / n.num);
	}
	double Number::get_num() {
		return num;
	}
	void Number::set_num(double n) {
		num = n;
	}


Number create_number(int n) {
	return Number(n);
}

void fnNumber()
{

}
