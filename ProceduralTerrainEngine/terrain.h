#pragma once

#include "terrainchunk.h"
#include "loader.h"
#include "camera.h"
#include "shader.h"
#include "light.h"

class Terrain
{
public:
	Terrain(Loader loader);
	~Terrain();

	void render(GLFWwindow* window, Camera camera, vector<Light> allLights);

private: 
	vector<TerrainChunk> chunks; 
	Shader shader;
	GLuint grassTexture, rockTexture, sandTexture;
};

