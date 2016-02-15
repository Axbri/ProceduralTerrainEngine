#pragma once

#include "terrainchunk.h"
#include "loader.h"
#include "camera.h"
#include "shader.h"
#include "light.h"
#include "vec2.h"

class Terrain
{
public:
	Terrain(Loader loader);
	~Terrain();

	void update(Loader loader, Camera camera);
	void render(GLFWwindow* window, Camera camera, vector<Light> allLights);
	
	Vec3 getChunkIndex(Vec3 pos); 
	int getNumberOfChunksLoaded(); 

private: 
	vector<TerrainChunk> chunks; 
	Shader shader;
	GLuint grassTexture, rockTexture, sandTexture;
};

