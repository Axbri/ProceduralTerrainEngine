#pragma once

#include <math.h>
#include <stdlib.h>

#include "vec3.h"
#include "SimplexNoise.h"

class TerrainHeightGenerator
{
public:
	
	static double getHeight(double xPos, double zPos);
	static Vec3 getNormal(double xPos, double zPos);
	/*
	static constexpr double AMPLITUDE = 5.0;
	static constexpr int OCTAVES = 5;
	static constexpr double ROUGHNESS = 0.1;
	*/
private: 

	static double getNoise(double x, double z);
	//static double getSmoothNoise(int x, int z); 

};

//const double TerrainHeightGenerator::AMPLITUDE = 10.0;
//const int TerrainHeightGenerator::SEED = 4564223;
