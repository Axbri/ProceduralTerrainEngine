#pragma once

#include "terrainchunk.h"
#include "loader.h"
#include "camera.h"
#include "shader.h"

class Terrain
{
public:
	Terrain(Loader loader);
	~Terrain();

	void render(GLFWwindow* window, Camera camera);

private: 
	vector<TerrainChunk> chunks; 
	Shader shader;
};

