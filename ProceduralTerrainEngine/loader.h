#ifndef LOADER_H_INCLUDED
#define LOADER_H_INCLUDED

#include <GL/glew.h> 
#include <GLFW/glfw3.h>  
#include <stdio.h>  
#include <stdlib.h> 
#include <sstream>
#include <vector>

#include "model.h"

using namespace std;

class Loader
{
public:
	Model createModel(float positions[], int size_pos, float textureCoords[], int size_texCoords, int indices[], int size_index);
	GLuint loadBMPtexture(const char * imagepath);
	void cleanUp(); 

private:
	GLuint createVao(); 
	void bindIndicesBuffer(int indices[], int n);
	void storeDataInAtributeList(int attributeNumber, int coordinateSize, float data[], int n);

	vector<GLuint> vaos;
	vector<GLuint> vbos;
	vector<GLuint> textures;
};

#endif
