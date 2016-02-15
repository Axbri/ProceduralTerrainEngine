#include "WindowSizeHandler.h"

Vec2 WindowSizeHandler::frameBufferSize = Vec2{ 1200, 800 };

void WindowSizeHandler::framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	frameBufferSize = Vec2{ (double)width, (double)height };
	glViewport(0, 0, width, height); 
}

Vec2 WindowSizeHandler::getFrameBufferSize()
{
	return frameBufferSize;
}
