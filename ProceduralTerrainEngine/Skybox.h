#pragma once

#include <GL/glew.h> 
#include <GLFW/glfw3.h>  
#include "camera.h"
#include "shader.h"
#include "loader.h"
#include "Settings.h"

class Skybox
{
public:
	Skybox(Loader loader);
	~Skybox();

	void render(GLFWwindow* window, Settings settings, Camera camera);
	void cleanUp(); 

private: 
	Shader shader; 
	Mat4 modelMatrix;
	GLuint texture; 
	GLuint model; 


};

