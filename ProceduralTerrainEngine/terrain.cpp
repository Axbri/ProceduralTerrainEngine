

#include "terrain.h"


Terrain::Terrain(Loader loader)
{
	shader = Shader();
	shader.createShader("terrain_vert.glsl", "terrain_frag.glsl");
	grassTexture = loader.loadBMPtexture("lushgrass.bmp");
	rockTexture = loader.loadBMPtexture("rock.bmp");
	sandTexture = loader.loadBMPtexture("sand.bmp");

	for (int x{ -LOADING_DISTANCE }; x < LOADING_DISTANCE; x++)
	{
		for (int z{ -LOADING_DISTANCE }; z < LOADING_DISTANCE; z++)
		{
			Vec3 offset{ x, 0, z };
			if (offset.lengthSquared() < LOADING_DISTANCE*LOADING_DISTANCE)
			{
				addToQueue(offset);
			}
		}
	}
}

Terrain::~Terrain()
{
	shader.cleanUp(); 
}

void Terrain::update(Loader loader, Player player)
{
	Vec3 cameraChunkIndex = getChunkIndex(player.getPosition());
	if (player.getVelocity().manhattanLength() > 0.01)
	{
		for (int x{ -LOADING_DISTANCE }; x < LOADING_DISTANCE; x++)
		{
			for (int z{ -LOADING_DISTANCE }; z < LOADING_DISTANCE; z++)
			{
				Vec3 offset{ x, 0, z };
				if (offset.lengthSquared() < LOADING_DISTANCE*LOADING_DISTANCE)
				{
					addToQueue(cameraChunkIndex + offset);
				}
			}
		}
	}

	// create one new chunk from the chunk creation queue.
	if (queue.size() != 0)
	{
		Vec3 newChunkIndex = queue.back();
		queue.pop_back();

		TerrainChunk newChunk = TerrainChunk(newChunkIndex.x, newChunkIndex.z);
		if (!(find(chunks.begin(), chunks.end(), newChunk) != chunks.end()))
		{
			newChunk.load(loader);
			chunks.push_back(newChunk);
		}
	}

	// remove chunks that are to far away
	for (int i{ 0 }; i < chunks.size(); i++)
	{
		double chunkToCameraDistance = (cameraChunkIndex - chunks[i].getIndex()).lengthSquared();				
		if (chunkToCameraDistance > (LOADING_DISTANCE*LOADING_DISTANCE+4))
		{
			chunks.erase(chunks.begin() + i);
			//break; 
		}
	}
}

void Terrain::render(GLFWwindow* window, Camera camera, vector<Light> allLights, Vec4 clipPlane)
{
	shader.start();
	shader.setUniformVec4("clipPlane", clipPlane);
	shader.setUniformMat4("projectionMatrix", camera.getProjectionMatrix());
	shader.setUniformMat4("viewMatrix", camera.getViewMatrix());
	Light::loadLightsToShader(shader, allLights); 
	shader.setUniformInt("grassTexture", 0);
	shader.setUniformInt("rockTexture", 1);
	shader.setUniformInt("sandTexture", 2);
	for (auto chunk : chunks)
	{
		Model chunkModel = chunk.getModel(camera);
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
	return Vec3(floor(pos.x / TerrainChunk::SIZE), 0.0, floor(pos.z / TerrainChunk::SIZE));
}

int Terrain::getNumberOfChunksLoaded()
{
	return chunks.size();
}

int Terrain::getQueueSize()
{
	return queue.size();
}

void Terrain::addToQueue(Vec3 pos)
{
	if (!(find(queue.begin(), queue.end(), pos) != queue.end()))
	{
		queue.insert(queue.begin(), pos);
	}	
}
