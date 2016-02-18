
#include <GL/glew.h> 
#include <GLFW/glfw3.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>

#include "loader.h"
#include "camera.h"
#include "shader.h"
#include "userinput.h"
#include "terrain.h"
#include "light.h"
#include "font.h"
#include "Player.h"
#include "Water.h"
#include "WindowSizeHandler.h"
#include "Skybox.h"
#include "Settings.h"

// Define an error callback  
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

// This function inizializes GLFW and OpenGL and 
// opens a window and returns a pointer to the window. 
GLFWwindow* createWindow()
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

	Vec2 size = WindowSizeHandler::getFrameBufferSize();
	window = glfwCreateWindow(size.x, size.y, "Procedural Terrain Engine - a demo by Axel Brinkeby", NULL, NULL);

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
	glfwSetFramebufferSizeCallback(window, WindowSizeHandler::framebuffer_size_callback);

	//Initialize GLEW    
	glewExperimental = GL_TRUE;
	if (GLenum err = glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
	// print information about the computer 
	cout << "GLFW version                : " << glfwGetVersionString() << endl;
	cout << "GLEW_VERSION                : " << glewGetString(GLEW_VERSION) << endl;
	cout << "GL_VERSION                  : " << glGetString(GL_VERSION) << endl;
	cout << "GL_VENDOR                   : " << glGetString(GL_VENDOR) << endl;
	cout << "GL_RENDERER                 : " << glGetString(GL_RENDERER) << endl;
	cout << "GL_SHADING_LANGUAGE_VERSION : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl << endl;
	return window;
}

// This is the main function that starts the program. 
int main(void)
{
	GLFWwindow* window = createWindow();

	Loader loader;
	Skybox sky{ loader };
	Font font{ loader, 0.015 };
	Terrain terrain{ loader };
	Player player{ 0, 0 };
	vector<Light> allLights;
	Water water{ loader };
	Settings settings; 

	// one light realy far away (without attenuation)
	allLights.push_back(Light{ 300000, 100000, -300000 });
	allLights[0].color = Vec3(0.5, 0.5, 0.5);

	allLights.push_back(Light{ 300000, 500000, -300000 });
	allLights[0].color = Vec3(0.5, 0.5, 0.5);

	// set the backgorund color and enable depth testing
	glClearColor(settings.getFogColor().x, settings.getFogColor().y, settings.getFogColor().z, 0.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// variables used in the main loop 
	double previus_time = 0, delta_time = 0, accumulated_time{ 0 };
	int framerate{ 0 }, frames_this_second{ 0 };

	do //Main Loop  
	{
		// ================================== update ==================================
		player.update(window, delta_time);
		
		if (UserInput::pollKey(window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (UserInput::getRightMouseButton()) {
			UserInput::setCursorLocked(window, false); 
		}

		if (UserInput::getLeftMouseButton()) {
			UserInput::setCursorLocked(window, true);
		}

		terrain.update(loader, player); 
		Vec3 chunkindex = terrain.getChunkIndex(player.getCamera().getPosition());

		// ================================== render ==================================
		
		
		// enable wireframe rendering if the user hold down the W key on the keyboard 
		if (UserInput::pollKey(window, GLFW_KEY_Q))
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glEnable(GL_CULL_FACE);
		

		// render reflection texture
		glEnable(GL_CLIP_DISTANCE0);
		water.bindReflectionBuffer(); 
		Camera tempCamera = player.getCamera(); 
		tempCamera.moveYpos(-2 * tempCamera.getPosition().y);
		tempCamera.invertTilt();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		sky.render(window, settings, tempCamera);
		terrain.render(window, settings, tempCamera, allLights, water.getReflectionClipPlane());
		

		// render refraction texture
		water.bindRefractionBuffer(); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		sky.render(window, settings, player.getCamera());
		terrain.render(window, settings, player.getCamera(), allLights, water.getRefractionClipPlane());
		

		// render the normal scene
		FrameBufferUtilities::unbindCurrentBuffer(); 
		glDisable(GL_CLIP_DISTANCE0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		sky.render(window, settings, player.getCamera());
		
		terrain.render(window, settings, player.getCamera(), allLights, water.getReflectionClipPlane());
		water.render(window, player.getCamera(), allLights);
		

		// wireframe rendering is of be default. 
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// render all the text: 
		font.setColor(0, 0, 0); 
		font.render("Frame rate", framerate, 0.75, 0.98);
		//font.render("you are in chunk", chunkindex, -0.95, 0.95);
		//font.render("number of loaded chunks", terrain.getNumberOfChunksLoaded(), -0.95, 0.90);
		//font.render("number of chunks i generation queue", terrain.getQueueSize(), -0.95, 0.85);
		//font.render("player pos", player.getPosition(), -0.95, 0.80);
		//font.render("palyer vel", player.getVelocity(), -0.95, 0.75);

		//Swap buffers  
		glfwSwapBuffers(window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...  
		glfwPollEvents();

		// ======================= calculate deltaTime and framerate ========================
		// calculate delta time, used for simulating accurate physics and showing framrate
		delta_time = glfwGetTime() - previus_time;
		previus_time = glfwGetTime();
		
		accumulated_time += delta_time;
		frames_this_second++;
		if (accumulated_time > 1)
		{
			accumulated_time -= 1;
			framerate = frames_this_second;
			frames_this_second = 0;
		}

	} while (!glfwWindowShouldClose(window));

	water.cleanUp(); 
	//terrain.cleanUp(); 
	loader.cleanUp();
	//Close OpenGL window and terminate GLFW  
	glfwDestroyWindow(window);
	//Finalize and clean up GLFW  
	glfwTerminate();
	exit(EXIT_SUCCESS);
}