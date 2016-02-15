#pragma once

#include "model.h"
#include "terrainheightgenerator.h"
#include "loader.h"
#include "camera.h"

class TerrainChunk
{

public:
	TerrainChunk(); 
	TerrainChunk(Loader loader, int x, int z);
	~TerrainChunk();
	Model getModel(Camera camera); 

	static const int SIZE = 16;						// length of the terrains chunk's sides in OpenGL units
	static const int NUMBER_OF_VERTICES = 33;		// number of vertices in along the sides

private: 
	
	double xIndex;
	double zIndex;
	double xPos; 
	double zPos; 
	Model models[3]; 
};

