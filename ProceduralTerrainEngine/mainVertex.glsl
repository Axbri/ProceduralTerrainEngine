#version 330 core

layout (location = 0) in vec3 position; 
layout (location = 1) in vec2 textureCoords; 

out vec2 interpolatedTextureCoords; 

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main (void) 
{

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);		
	//gl_Position = vec4(position, 1.0); 
	
	interpolatedTextureCoords = textureCoords; 

}