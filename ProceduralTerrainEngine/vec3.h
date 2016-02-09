#pragma once

#include <math.h>

class Vec3
{
public:
	Vec3();
	Vec3(double xValue, double yValue, double zValue);

	void normalize();
	double length(); 

	double x, y, z; 
};

