#pragma once
#include "../Number/Number.h"
#include <cmath>

 
class Vector {
	Number x, y;
public:
		Vector();
	Vector(int x, int y);
	Vector(Number x, Number y);
	Number polar_r();
	Number polar_phi();
	Vector operator+(const Vector& vec);
};