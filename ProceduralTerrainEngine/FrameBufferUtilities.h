#pragma once

#include <GL/glew.h> 
#include <GLFW/glfw3.h>  
#include "WindowSizeHandler.h"

class FrameBufferUtilities
{
public:
	static GLuint createBuffer(); 
	static GLuint createTextureAttachment(int width, int height);
	static GLuint createDepthTextureAttachment(int width, int height);
	static GLuint createDepthBufferAttachment(int width, int height);
	static void bindBuffer(GLuint frameBuffer, int width, int height);
	static void unbindCurrentBuffer();

};

