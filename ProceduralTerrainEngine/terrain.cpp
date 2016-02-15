

#include "terrain.h"


Terrain::Terrain(Loader loader)
{
	shader = Shader();
	shader.createShader("terrain_vert.glsl", "terrain_frag.glsl");
	grassTexture = loader.loadBMPtexture("lushgrass.bmp");
	rockTexture = loader.loadBMPtexture("rock.bmp");
	sandTexture = loader.loadBMPtexture("sand.bmp");

	TerrainChunk chunk1 = TerrainChunk(loader, 0, 0);
	TerrainChunk chunk2 = TerrainChunk(loader, 1, 0);
	TerrainChunk chunk3 = TerrainChunk(loader, 2, 0);

	chunks.push_back(chunk1);
	chunks.push_back(chunk2);
	chunks.push_back(chunk3);

	/*
	for (int x = 0; x < 9; x++)
	{
		for (int z = 0; z < 9; z++)
		{
			TerrainChunk chunk = TerrainChunk(loader, x * TerrainChunk::SIZE - 72, z * TerrainChunk::SIZE - 72);					
			chunks.push_back(chunk);
		}
	}	*/
}

Terrain::~Terrain()
{
	shader.cleanUp(); 
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
