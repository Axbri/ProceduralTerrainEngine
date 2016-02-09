
#include <GL/glew.h> 
#include <GLFW/glfw3.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <sstream>
#include <iostream>

#include "loader.h"
#include "camera.h"
#include "shader.h"
#include "userinput.h"
#include "terrain.h"

// Define an error callback  
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

// This function inizializes GLFW and OpenGL and 
// opens a window and returns a pointer to the window. 
GLFWwindow* init()
{
	//Set the error callback  
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW  
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version  
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing  
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Declare a window object  
	GLFWwindow* window;

	//Fullscreen:
	//const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	//int window_width = mode->width;
	//int window_height = mode->height;
	//window = glfwCreateWindow(window_width, window_height, "Test Window", glfwGetPrimaryMonitor(), NULL);

	window = glfwCreateWindow(1600, 1000, "Test Window", NULL, NULL);

	//If the window couldn't be created  
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread.   
	glfwMakeContextCurrent(window);

	//Sets callback functions for user input
	glfwSetKeyCallback(window, UserInput::key_callback);
	glfwSetCursorPosCallback(window, UserInput::mouse_pos_callback);
	glfwSetMouseButtonCallback(window, UserInput::mouse_button_callback);
	glfwSetScrollCallback(window, UserInput::scroll_callback);

	//Initialize GLEW    
	if (GLenum err = glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
	return window;
}

// Render a model
void render_model(GLFWwindow* window, Model model)
{
	glBindVertexArray(model.get_id());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model.get_texture());

	glDrawElements(GL_TRIANGLES, model.get_vertexcount(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindVertexArray(0);
}

// close the GLFW-windoe and tenminate GLFW
void cleanup(GLFWwindow* window)
{
	//Close OpenGL window and terminate GLFW  
	glfwDestroyWindow(window);
	//Finalize and clean up GLFW  
	glfwTerminate();
}

// This is the main function that starts the program. 
int main(void)
{
	GLFWwindow* window = init();			// init GLFW and GLEW

	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	cout << "window height: " << windowWidth << "px, window width: " << windowHeight << "px " << endl;

	Loader loader;
	Terrain terrain = Terrain(loader); 
	Camera camera = Camera((float)windowHeight / (float)windowWidth);

	// set the backgorund color and enable depth testing
	glClearColor(0.4f, 0.6f, 0.7f, 0.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	// variables used in the main loop 
	double previus_time = 0, delta_time = 0;

	do //Main Loop  
	{
		// ================================== update ==================================
		camera.update(delta_time);
		
		// ================================== render ==================================
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if (UserInput::getRightMouseButton())
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		terrain.render(window, camera);
		
		//Swap buffers  
		glfwSwapBuffers(window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...  
		glfwPollEvents();

		// ======================= calculate deltaTime and framerate ========================
		// calculate delta time, used for simulating accurate physics and showing framrate
		delta_time = glfwGetTime() - previus_time;
		previus_time = glfwGetTime();

		// show framerate in the title of the window
		std::ostringstream strs;
		strs << "Framerate: " << (int)(1 / delta_time) << "FPS";
		glfwSetWindowTitle(window, strs.str().c_str());

	} while (!glfwWindowShouldClose(window));

	//mainShader.cleanUp();
	loader.cleanUp();
	cleanup(window);
	exit(EXIT_SUCCESS);
}