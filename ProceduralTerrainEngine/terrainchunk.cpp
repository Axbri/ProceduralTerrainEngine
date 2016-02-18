#include "terrainchunk.h"

TerrainChunk::TerrainChunk(int x, int z)
{
	index = Vec3{ x, 0, z }; 
	pos = Vec3{ x * SIZE, 0, z * SIZE };	
	loaded[0] = false; 
	loaded[1] = false;
	loaded[2] = false;
	requestedLoaded[0] = false;
	requestedLoaded[1] = false;
	requestedLoaded[2] = true;
}

TerrainChunk::~TerrainChunk()
{

}

bool TerrainChunk::load(Loader loader)
{
	for (int i{ 2 }; i >= 0; i--)
	{
		if (requestedLoaded[i])
		{
			if (!loaded[i])
			{
				loadLODlevel(loader, i);
				loaded[i] = true;
				return true;
			}
		}
	}
	return false; 
}

Model TerrainChunk::getModel(Camera camera)
{
	Vec3 cameraToChunk = camera.getPosition() - Vec3{ pos.x + SIZE / 2 , 0.0, pos.z + SIZE / 2 };
	double distance = cameraToChunk.length(); 

	if (distance < SIZE * 2)
	{
		requestedLoaded[0] = true;
		if (loaded[0])
			return models[0];			
	}
		
	if (distance < SIZE * 3)
	{
		requestedLoaded[1] = true;
		if (loaded[1])
			return models[1];			
	}

	return models[2];
}

Vec3 TerrainChunk::getIndex()
{
	return index;
}

Vec3 TerrainChunk::getPosition()
{
	return pos;
}

bool TerrainChunk::operator==(const TerrainChunk & otherChunk) const
{
	return (index == otherChunk.index);
}

void TerrainChunk::loadLODlevel(Loader loader, int level)
{
	const int numberOfVertices[] = { NUMBER_OF_VERTICES , NUMBER_OF_VERTICES / 2 + 1, NUMBER_OF_VERTICES / 4 + 1 };

	int indices[(NUMBER_OF_VERTICES - 1) * (NUMBER_OF_VERTICES - 1) * 6];
	float positions[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 3];
	float textureCoords[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 2];
	float normals[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 3];
	int indicesIndex = 0, positionIndex = 0, textureCoordIndex = 0, normalIndex = 0;

	for (int x = 0; x < numberOfVertices[level]; x++)
	{
		for (int z = 0; z < numberOfVertices[level]; z++)
		{
			float localVertexPosX = (float)(x * SIZE) / (float)(numberOfVertices[level] - 1);
			float localVertexPosZ = (float)(z * SIZE) / (float)(numberOfVertices[level] - 1);
			double height = TerrainHeightGenerator::getHeight(pos.x + localVertexPosX, pos.z + localVertexPosZ);

			if (x == 0 || z == 0 || x == numberOfVertices[level] - 1 || z == numberOfVertices[level] - 1)
			{
				if (level == 1)
					height -= 0.15;
				else if (level == 2)
					height -= 0.30;
			}

			positions[positionIndex++] = pos.x + localVertexPosX;
			positions[positionIndex++] = (float)height;
			positions[positionIndex++] = pos.z + localVertexPosZ;

			float texCoordU = (float)(x) / (float)(numberOfVertices[level] - 1);
			float texCoordV = (float)(z) / (float)(numberOfVertices[level] - 1);
			textureCoords[textureCoordIndex++] = texCoordU;
			textureCoords[textureCoordIndex++] = texCoordV;

			Vec3 normal = TerrainHeightGenerator::getNormal(pos.x + localVertexPosX, pos.z + localVertexPosZ);
			normals[normalIndex++] = normal.x;
			normals[normalIndex++] = normal.y;
			normals[normalIndex++] = normal.z;
		}
	}

	for (int x = 0; x < numberOfVertices[level] - 1; x++)
	{
		for (int z = 0; z < numberOfVertices[level] - 1; z++)
		{
			int topLeft = (z * numberOfVertices[level]) + x;
			int topRight = topLeft + 1;
			int bottomLeft = ((z + 1) * numberOfVertices[level]) + x;
			int bottomRight = bottomLeft + 1;
			indices[indicesIndex++] = topLeft;
			indices[indicesIndex++] = bottomLeft;
			indices[indicesIndex++] = topRight;
			indices[indicesIndex++] = topRight;
			indices[indicesIndex++] = bottomLeft;
			indices[indicesIndex++] = bottomRight;
		}
	}
	models[level] = loader.createModel(positions, positionIndex, textureCoords, textureCoordIndex, normals, normalIndex, indices, indicesIndex);
}
