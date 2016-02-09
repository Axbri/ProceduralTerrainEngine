#include "model.h"

using namespace std; 

Model::Model()
{
	vao_id = vertex_count = 0;
	MatrixMath::identityMat4(modelMatrix);
}

Model::Model(const int id, const int count)
{
	vao_id = id;
	vertex_count = count;
	MatrixMath::identityMat4(modelMatrix);
}

GLuint Model::get_id()
{
	return vao_id;
}

GLuint Model::get_texture()
{
	return texture_id; 
}

void Model::getModelMatrix(float matrix[])
{
	for (int i = 0; i < 16; i++) {
		matrix[i] = modelMatrix[i];
	}
}

void Model::setPosition(float x, float y, float z)
{
	MatrixMath::translateMat4(modelMatrix, x, y, z);
}

void Model::setScale(float sx, float sy, float sz)
{
	MatrixMath::scaleMat4(modelMatrix, sx, sy, sz);
}

void Model::set_texture(GLuint texture)
{
	texture_id = texture;
}

int Model::get_vertexcount()
{
	return vertex_count;
}