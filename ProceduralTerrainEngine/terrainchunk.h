#pragma once

#include "model.h"
#include "terrainheightgenerator.h"
#include "loader.h"

class TerrainChunk
{

public:
	TerrainChunk(); 
	TerrainChunk(Loader loader, double x, double z);
	~TerrainChunk();
	Model getModel(); 

	static const int SIZE = 16;						// length of the terrains chunk's sides in OpenGL units
	static const int NUMBER_OF_VERTICES = 32;		// number of vertices in along the sides

private: 
	
	double xPos; 
	double zPos; 
	double vertexHeights[NUMBER_OF_VERTICES][NUMBER_OF_VERTICES];		// NUMBER_OF_VERTICES 
	Model model; 

};

