

#include "terrain.h"


Terrain::Terrain(Loader loader)
{
	shader = Shader();
	shader.createShader("terrain_vert.glsl", "terrain_frag.glsl");
	grassTexture = loader.loadBMPtexture("lushgrass.bmp");
	rockTexture = loader.loadBMPtexture("rock.bmp");
	sandTexture = loader.loadBMPtexture("sand.bmp");

	TerrainChunk chunk1 = TerrainChunk(loader, -1, 0);
	TerrainChunk chunk2 = TerrainChunk(loader, -1, -1);
	TerrainChunk chunk3 = TerrainChunk(loader, 0, 0);
	TerrainChunk chunk4 = TerrainChunk(loader, 0, -1);

	chunks.push_back(chunk1);
	chunks.push_back(chunk2);
	chunks.push_back(chunk3); 
	chunks.push_back(chunk4);
}

Terrain::~Terrain()
{
	shader.cleanUp(); 
}

void Terrain::update(Loader loader, Camera camera)
{
	Vec3 chunkindex = getChunkIndex(camera.getPosition());
	bool chunkExists = false; 

	for (int i = 0; i < chunks.size(); i++)
	{		
		if (chunks[i].getIndex().x == chunkindex.x && 
			chunks[i].getIndex().z == chunkindex.z)
		{
			chunkExists = true; 
		}

		if (chunks[i].getIndex().x > chunkindex.x + 1 || chunks[i].getIndex().x < chunkindex.x - 1 ||
			chunks[i].getIndex().z > chunkindex.z + 1 || chunks[i].getIndex().z < chunkindex.z - 1)
		{
			chunks.erase(chunks.begin() + i);
		}
	}

	if (!chunkExists)
	{
		TerrainChunk newChunk = TerrainChunk(loader, chunkindex.x, chunkindex.z);
		chunks.push_back(newChunk);
	}
}

void Terrain::render(GLFWwindow* window, Camera camera, vector<Light> allLights)
{
	shader.start();
	shader.setUniformMat4("projectionMatrix", camera.getProjectionMatrix());
	shader.setUniformMat4("viewMatrix", camera.getViewMatrix());
	Light::loadLightsToShader(shader, allLights); 
	shader.setUniformInt("grassTexture", 0);
	shader.setUniformInt("rockTexture", 1);
	shader.setUniformInt("sandTexture", 2);
	
	for (int i = 0; i < chunks.size(); i++)
	{
		Model chunkModel = chunks[i].getModel(camera);
		shader.setUniformMat4("modelMatrix", chunkModel.getModelMatrix());
		
		glBindVertexArray(chunkModel.get_id());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, grassTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, rockTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, sandTexture);

		glDrawElements(GL_TRIANGLES, chunkModel.get_vertexcount(), GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);
	}
	
	shader.stop();
}

Vec3 Terrain::getChunkIndex(Vec3 pos)
{
	int xIndex = floor(pos.x / TerrainChunk::SIZE);
	int zIndex = floor(pos.z / TerrainChunk::SIZE);
	return Vec3(xIndex, 0, zIndex);
}

int Terrain::getNumberOfChunksLoaded()
{
	return chunks.size();
}
