#pragma once

class Number {
	double num;
public:
	Number(double num);
	Number();
	Number operator+(const Number& n);
	Number operator-(const Number& n);
	Number operator*(const Number& n);
	Number operator/(const Number& n);
	double get_num();
	void set_num(double n);
};

const Number NUMBER_ZERO = Number(0);
const Number NUMBER_ONE = Number(1);

Number create_number(int n);