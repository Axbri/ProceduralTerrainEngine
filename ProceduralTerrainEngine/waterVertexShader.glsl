#version 330 core

layout (location = 0) in vec3 position;

out vec2 textureCoords;
out vec3 toCameraVector; 
out vec3 toLightVector[8]; 
out vec4 clipSpace; 

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 lightPosition[8]; 


void main(void) {

	vec4 worldPosition = modelMatrix * vec4(position, 1.0);	
	worldPosition.y = -0.07; 
	vec4 transformedPos = projectionMatrix * viewMatrix * worldPosition;
 
	gl_Position = transformedPos; 	
	clipSpace = transformedPos; 	
		
	toCameraVector = vec3(inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)) - vec3(worldPosition); 	
	textureCoords = vec2(worldPosition.x, worldPosition.z) / 300;
	
	for (int i = 0; i < 8; i++) {
		toLightVector[i] = lightPosition[i] - vec3(worldPosition); 
	}
 
}