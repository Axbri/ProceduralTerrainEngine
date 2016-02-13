

#include "terrainchunk.h"


TerrainChunk::TerrainChunk()
{

}

TerrainChunk::TerrainChunk(Loader loader, double x, double z)
{
	xPos = x; 
	zPos = z; 
	
	const int numberOfVertices[] = { NUMBER_OF_VERTICES , NUMBER_OF_VERTICES / 2 + 1, NUMBER_OF_VERTICES / 4 + 1}; 


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
				double height = TerrainHeightGenerator::getHeight(xPos + localVertexPosX, zPos + localVertexPosZ);
				vertexHeights[x][z] = height;
				positions[positionIndex++] = xPos + localVertexPosX;
				
				
				if (x == 0 || z == 0 || x == numberOfVertices[i]-1 || z == numberOfVertices[i]-1)
				{
					if (i == 1)
						height -= 0.1;
					else if (i == 2)
						height -= 0.5;
				}

				

				positions[positionIndex++] = (float)height;


				positions[positionIndex++] = zPos + localVertexPosZ;

				float texCoordU = (float)(x) / (float)numberOfVertices[i];
				float texCoordV = (float)(z) / (float)numberOfVertices[i];
				textureCoords[textureCoordIndex++] = texCoordU;
				textureCoords[textureCoordIndex++] = texCoordV;

				Vec3 normal = TerrainHeightGenerator::getNormal(xPos + localVertexPosX, zPos + localVertexPosZ);
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

TerrainChunk::~TerrainChunk()
{

}

Model TerrainChunk::getModel(Camera camera)
{
	Vec3 cameraToChunk = camera.getPosition() - Vec3{ xPos + SIZE / 2 , 0, zPos + SIZE / 2 };

	double distance = cameraToChunk.length(); 

	if (distance < 32)
		return models[0];

	if (distance < 64)
		return models[1];

	return models[2];

}
