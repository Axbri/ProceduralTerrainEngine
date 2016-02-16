

#include "terrainchunk.h"


TerrainChunk::TerrainChunk(int x, int z)
{
	index = Vec3{ x, 0, z }; 
	pos = Vec3{ x * SIZE, 0, z * SIZE };
}

TerrainChunk::~TerrainChunk()
{

}

void TerrainChunk::load(Loader loader)
{
	const int numberOfVertices[] = { NUMBER_OF_VERTICES , NUMBER_OF_VERTICES / 2 + 1, NUMBER_OF_VERTICES / 4 + 1 };

	for (int i{ 0 }; i < 3; i++)
	{
		int indices[(NUMBER_OF_VERTICES - 1) * (NUMBER_OF_VERTICES - 1) * 6];
		float positions[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 3];
		float textureCoords[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 2];
		float normals[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 3];
		int indicesIndex = 0, positionIndex = 0, textureCoordIndex = 0, normalIndex = 0;

		for (int x = 0; x < numberOfVertices[i]; x++)
		{
			for (int z = 0; z < numberOfVertices[i]; z++)
			{
				float localVertexPosX = (float)(x * SIZE) / (float)(numberOfVertices[i] - 1);
				float localVertexPosZ = (float)(z * SIZE) / (float)(numberOfVertices[i] - 1);
				double height = TerrainHeightGenerator::getHeight(pos.x + localVertexPosX, pos.z + localVertexPosZ);

				if (x == 0 || z == 0 || x == numberOfVertices[i] - 1 || z == numberOfVertices[i] - 1)
				{
					if (i == 1)
						height -= 0.06;
					else if (i == 2)
						height -= 0.2;
				}

				positions[positionIndex++] = pos.x + localVertexPosX;
				positions[positionIndex++] = (float)height;
				positions[positionIndex++] = pos.z + localVertexPosZ;

				float texCoordU = (float)(x) / (float)(numberOfVertices[i] - 1);
				float texCoordV = (float)(z) / (float)(numberOfVertices[i] - 1);
				textureCoords[textureCoordIndex++] = texCoordU;
				textureCoords[textureCoordIndex++] = texCoordV;

				Vec3 normal = TerrainHeightGenerator::getNormal(pos.x + localVertexPosX, pos.z + localVertexPosZ);
				normals[normalIndex++] = normal.x;
				normals[normalIndex++] = normal.y;
				normals[normalIndex++] = normal.z;
			}
		}

		for (int x = 0; x < numberOfVertices[i] - 1; x++)
		{
			for (int z = 0; z < numberOfVertices[i] - 1; z++)
			{
				int topLeft = (z * numberOfVertices[i]) + x;
				int topRight = topLeft + 1;
				int bottomLeft = ((z + 1) * numberOfVertices[i]) + x;
				int bottomRight = bottomLeft + 1;
				indices[indicesIndex++] = topLeft;
				indices[indicesIndex++] = bottomLeft;
				indices[indicesIndex++] = topRight;
				indices[indicesIndex++] = topRight;
				indices[indicesIndex++] = bottomLeft;
				indices[indicesIndex++] = bottomRight;
			}
		}
		models[i] = loader.createModel(positions, positionIndex, textureCoords, textureCoordIndex, normals, normalIndex, indices, indicesIndex);
	}
}

Model TerrainChunk::getModel(Camera camera)
{
	Vec3 cameraToChunk = camera.getPosition() - Vec3{ pos.x + SIZE / 2 , 0.0, pos.z + SIZE / 2 };
	double distance = cameraToChunk.length(); 
	if (distance < 32)
		return models[0];
	if (distance < 64)
		return models[1];
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
