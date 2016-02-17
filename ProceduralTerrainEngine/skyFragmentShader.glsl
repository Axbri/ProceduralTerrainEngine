# version 330 core

out vec4 pixelColor;

in vec3 interpolatedTextureCoordinates;

uniform samplerCube cubeMap1;
uniform float gamma; 
uniform vec3 fogColor;
uniform float lowerLimit; 
uniform float upperLimit; 

void main (){	
	vec3 texture1Color = texture(cubeMap1, interpolatedTextureCoordinates).rgb; 
	
	//float mixAmount = clamp((interpolatedTextureCoordinates.y - lowerLimit) / (upperLimit - lowerLimit), 0.0, 1.0);		
	
	//vec3 finalColor = mix(fogColor, texture1Color, mixAmount);  
	
	//pixelColor = vec4(pow(finalColor, vec3(1.0 / gamma)), 1.0);
	pixelColor = vec4(texture1Color, 1.0); 
}

