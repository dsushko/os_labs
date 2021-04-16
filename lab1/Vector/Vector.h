#pragma once
#include "Number.h"
#include <cmath>

#ifdef VECTOR_EXPORTS
#define VECTOR_API __declspec(dllexport)
#else
#define VECTOR_API __declspec(dllimport)
#endif
 
class Vector {
	Number x, y;
public:
	VECTOR_API	Vector();
	VECTOR_API	Vector(int x, int y);
	VECTOR_API	Vector(Number x, Number y);
	VECTOR_API	Number polar_r();
	VECTOR_API	Number polar_phi();
	VECTOR_API	Vector operator+(const Vector& vec);
};