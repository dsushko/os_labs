#include "Vector.h"

Vector::Vector() {
	x = Number(0);
	y = Number(0);
}
Vector::Vector(int x, int y) {
	this->x = Number(x);
	this->y = Number(y);
}
Vector::Vector(Number x, Number y) {
	this->x = x;
	this->y = y;
}
Number Vector::polar_r() {
	Number n = x * x + y * y;
	return Number(sqrt(n.get_num()));
}
Number Vector::polar_phi() {
	Number n = (y / x);
	return Number(atan(n.get_num()));
}
Vector Vector::operator+(const Vector& vec) {
	return Vector(x + vec.x, y + vec.y);
}
