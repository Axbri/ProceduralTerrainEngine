#include "terrainheightgenerator.h"

double TerrainHeightGenerator::getHeight(double xPos, double zPos)
{
	double AMPLITUDE = 20;
	int OCTAVES = 10;
	double ROUGHNESS = 0.4;
	

	double total{ 0 };
	double d = pow(2, OCTAVES - 1);
	for (int i{ 0 }; i < OCTAVES; i++)
	{
		double freq = pow(2, i) / d;
		double amplitude = pow(ROUGHNESS, i) * AMPLITUDE;
		total += getNoise(xPos * freq, zPos * freq) * amplitude;
	}

	return total + AMPLITUDE/2.0;
}

Vec3 TerrainHeightGenerator::getNormal(double xPos, double zPos)
{
	double delta = 0.5; 
	double heightVest = getHeight(xPos - delta, zPos);
	double heightEast = getHeight(xPos + delta, zPos);
	double heightSouth = getHeight(xPos, zPos - delta);
	double heightNorth = getHeight(xPos, zPos + delta);	

	Vec3 normal = Vec3(heightVest - heightEast, 2.0, heightSouth - heightNorth);
	normal.normalize(); 
	return normal; 
}

double TerrainHeightGenerator::getNoise(double x, double z)
{
	//srand(x * 53737833 + z * 2757536 + SEED);
	//return ((rand() / (double)RAND_MAX) * 2.0) - 1.0;
	double SCALE = 0.8;
	return SimplexNoise::noise((float)x / SCALE, (float)z / SCALE);
}

/*
double TerrainHeightGenerator::getSmoothNoise(int x, int z)
{
	double corners = (getNoise(x-1, z-1) + getNoise(x+1, z-1) + getNoise(x-1, z+1) + getNoise(x+1, z+1)) / 16.0;
	double sides = (getNoise(x, z+1) + getNoise(x+1, z) + getNoise(x, z-1) + getNoise(x, z+1)) / 8.0;
	double center = getNoise(x, z) / 2.0;
	return corners + sides + center;
}
*/