#version 330 core

in vec2 interpolatedTextureCoords; 

out vec4 pixel_color; 

uniform sampler2D mainTexture; 

void main (void) 
{

	pixel_color = texture(mainTexture, interpolatedTextureCoords * 16); //vec4(1, 0+interpolatedTextureCoords.x, 0, 1);  

}