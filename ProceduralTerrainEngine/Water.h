#pragma once

#include <GL/glew.h> 
#include <GLFW/glfw3.h>  
#include "FrameBufferUtilities.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "loader.h"
#include "shader.h"


class Water
{
public:
	Water(Loader loader);
	~Water();

	
	void cleanUp(); 
	void bindReflectionBuffer(); 
	void bindRefractionBuffer(); 

	void render(GLFWwindow* window, Camera camera, vector<Light> allLights);
		
	Vec4 getReflectionClipPlane(); 
	Vec4 getRefractionClipPlane();

private: 

	const int REFLECTION_WIDTH = 1280;
	const int REFLECTION_HEIGHT = 720;
	const int REFRACTION_WIDTH = 1280;
	const int REFRACTION_HEIGHT = 720;

	GLuint reflectionFrameBuffer; 
	GLuint reflectionTexture;
	GLuint reflectionDepthBuffer;

	GLuint refractionFrameBuffer;
	GLuint refractionTexture;
	GLuint refractionDepthTexture;

	GLuint dudvTexture; 
	GLuint normalTexture; 
	Model waterplane; 
	Shader shader; 

	Vec4 reflection_clip_plane; 
	Vec4 refraction_clip_plane; 

};

