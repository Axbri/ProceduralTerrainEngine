#pragma once

#include <math.h>

#include "vec3.h"

class TerrainHeightGenerator
{
public:
	
	static double getHeight(double xPos, double zPos);
	static Vec3 getNormal(double xPos, double zPos);

};

