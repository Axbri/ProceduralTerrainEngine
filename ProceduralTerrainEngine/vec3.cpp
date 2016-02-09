#include "vec3.h"

Vec3::Vec3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vec3::Vec3(double xValue, double yValue, double zValue)
{
	x = xValue;
	y = yValue;
	z = zValue;
}

void Vec3::normalize()
{
	float length = this->length();
	x *= 1.0 / length;
	y *= 1.0 / length;
	z *= 1.0 / length;
}

double Vec3::length() 
{
	return sqrt(x * x + y * y + z * z);
}