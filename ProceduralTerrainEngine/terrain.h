#pragma once

#include "terrainchunk.h"
#include "loader.h"
#include "camera.h"
#include "shader.h"
#include "light.h"
#include "player.h"
#include "vec2.h"

class Terrain
{
public:
	Terrain(Loader loader);
	~Terrain();

	void update(Loader loader, Player player);
	void render(GLFWwindow* window, Camera camera, vector<Light> allLights, Vec4 clipPlane);
	
	Vec3 getChunkIndex(Vec3 pos); 
	int getNumberOfChunksLoaded(); 
	int getQueueSize(); 

	const int LOADING_DISTANCE = 4; 

private: 

	void removeFarawayChunks();

	void addToQueue(Vec3 pos);

	vector<Vec3> queue;
	vector<TerrainChunk> chunks; 
	Shader shader;
	GLuint grassTexture, rockTexture, sandTexture;
};

