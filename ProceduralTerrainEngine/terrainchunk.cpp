

#include "terrainchunk.h"


TerrainChunk::TerrainChunk()
{

}

TerrainChunk::TerrainChunk(Loader loader, double x, double z)
{
	xPos = x; 
	zPos = z; 
	
	int indices[(NUMBER_OF_VERTICES - 1) * (NUMBER_OF_VERTICES - 1) * 6];
	float positions[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 3];	
	float textureCoords[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 2];
	float normals[NUMBER_OF_VERTICES * NUMBER_OF_VERTICES * 3];
	int indicesIndex = 0, positionIndex = 0, textureCoordIndex = 0, normalIndex = 0;
	
	for (int x = 0; x < NUMBER_OF_VERTICES; x++)
	{
		for (int z = 0; z < NUMBER_OF_VERTICES; z++)
		{
			float localVertexPosX = (float)(x * SIZE) / (float)(NUMBER_OF_VERTICES - 1);
			float localVertexPosZ = (float)(z * SIZE) / (float)(NUMBER_OF_VERTICES - 1);
			double height = TerrainHeightGenerator::getHeight(xPos + localVertexPosX, zPos + localVertexPosZ);
			vertexHeights[x][z] = height; 
			positions[positionIndex++] = xPos + localVertexPosX;
			positions[positionIndex++] = (float)height;
			positions[positionIndex++] = zPos + localVertexPosZ;

			float texCoordU = (float)(x) / (float)NUMBER_OF_VERTICES;
			float texCoordV = (float)(z) / (float)NUMBER_OF_VERTICES;
			textureCoords[textureCoordIndex++] = texCoordU;
			textureCoords[textureCoordIndex++] = texCoordV;

			Vec3 normal = TerrainHeightGenerator::getNormal(xPos + localVertexPosX, zPos + localVertexPosZ); 
			normals[normalIndex++] = xPos + localVertexPosX;
			normals[normalIndex++] = (float)height;
			normals[normalIndex++] = zPos + localVertexPosZ;
		}
	}

	for (int x = 0; x < NUMBER_OF_VERTICES - 1; x++)
	{
		for (int z = 0; z < NUMBER_OF_VERTICES - 1; z++) 
		{		
			int topLeft = (z * NUMBER_OF_VERTICES) + x;
			int topRight = topLeft + 1;
			int bottomLeft = ((z + 1) * NUMBER_OF_VERTICES) + x;
			int bottomRight = bottomLeft + 1;
			indices[indicesIndex++] = topLeft;
			indices[indicesIndex++] = bottomLeft;
			indices[indicesIndex++] = topRight;
			indices[indicesIndex++] = topRight;
			indices[indicesIndex++] = bottomLeft;
			indices[indicesIndex++] = bottomRight;
		}
	}

	model = loader.createModel(positions, positionIndex, textureCoords, textureCoordIndex, indices, indicesIndex);
}

TerrainChunk::~TerrainChunk()
{

}

Model TerrainChunk::getModel()
{
	return model;
}
