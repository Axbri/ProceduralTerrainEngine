#include "terrainheightgenerator.h"


double TerrainHeightGenerator::getHeight(double xPos, double zPos)
{
	return (sin(xPos) + sin(zPos)) / 2;
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
