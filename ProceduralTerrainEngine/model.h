#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <GL/glew.h> 
#include "matrixmath.h"
using namespace std;

class Model
{
	public:
		Model();
		Model(int h, int m);

		GLuint get_id();
		GLuint get_texture(); 
		void set_texture(GLuint texture);
		int get_vertexcount();
		void getModelMatrix(float matrix[]);
		void setPosition(float x, float y, float z); 
		void setScale(float sx, float sy, float sz);

	private: 
		GLuint vao_id;
		GLuint texture_id; 
		int vertex_count;
		GLfloat modelMatrix[16];
};
#endif