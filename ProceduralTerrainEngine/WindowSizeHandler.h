#pragma once

#include <GLFW/glfw3.h>  
#include "vec2.h"

class WindowSizeHandler
{
public:
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static Vec2 getFrameBufferSize();
private: 
	static Vec2 frameBufferSize; 
};

