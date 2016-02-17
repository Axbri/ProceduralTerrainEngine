#version 330 core

in vec2 textureCoords;
in vec3 toCameraVector; 
in vec3 toLightVector[8]; 
in vec4 clipSpace; 

out vec4 pixelColor;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D depthTexture;
uniform sampler2D dudvTexture;
uniform sampler2D normalTexture;
uniform float time; 
uniform vec3 lightColor[8]; 
uniform vec3 lightAttenuation[8];

const float amountOfRipple = 0.1; 
const int duduScale = 50;
const int normalScale = 50;
const float normalTimeScale = 0.5;

void main(void) {
	vec2 normalizedDeviceCoords = (clipSpace.xy/clipSpace.w) / 2.0 + 0.5; 
	
	float near = 0.01; 
	float far = 200; 
	float depth = texture(depthTexture, normalizedDeviceCoords).r; 
	float terrainDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near)); 	 
	depth = gl_FragCoord.z; 
	float surfaceDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near)); 
	float waterDepth = 	terrainDistance - surfaceDistance; 
	
	vec4 tex1 = texture(dudvTexture, vec2(textureCoords.x * duduScale + time/20, textureCoords.y * duduScale + time/10)); 
	vec4 tex2 = texture(dudvTexture, vec2(textureCoords.x * duduScale - time/40, textureCoords.y * duduScale - time/40)); 	
	vec4 dudvMapColor = mix(tex1, tex2, 0.5); 
	
	vec4 tex3 = texture(normalTexture, vec2(textureCoords.x * normalScale - time/20 * normalTimeScale, textureCoords.y * normalScale - time/20 * normalTimeScale)); 
	vec4 tex4 = texture(normalTexture, vec2(textureCoords.x * normalScale + time/80 * normalTimeScale, textureCoords.y * normalScale + time/10 * normalTimeScale)); 	
	vec4 normalMapColor = mix(tex3, tex4, 0.5); 
	
	vec2 ripple = (dudvMapColor.rg - 0.5) * amountOfRipple * clamp(waterDepth, 0.0, 1.0); 
		
	vec3 reflectionColor = texture(reflectionTexture, vec2(normalizedDeviceCoords.x, 1 - normalizedDeviceCoords.y) + ripple).rgb; 
	vec3 refractionColor = texture(refractionTexture, normalizedDeviceCoords + ripple).rgb; 
	
	vec3 normal = normalize(normalMapColor.rbg * 2 - 1);
	
	vec3 unitToCameraVector = normalize(toCameraVector);
	
	vec3 textureColor = mix(reflectionColor, refractionColor, dot(unitToCameraVector, normalize(vec3(normal.x, normal.y*4, normal.z))));
		
	vec3 totalSpecular = vec3(0.0); 
	vec3 unitToLightVector[8];
	for (int i = 0; i < 8; i++) {
		float distanceToLight = length(toLightVector[i]); 
		float attenuationFactor = lightAttenuation[i].x + (lightAttenuation[i].y * distanceToLight) + (lightAttenuation[i].z * distanceToLight * distanceToLight);
		unitToLightVector[i] = normalize(toLightVector[i]); 		 		
		vec3 lightDirection = -unitToLightVector[i]; 	
		vec3 reflectedLightDirection = reflect(lightDirection, normal); 	
		float specularFactor = max(dot(unitToCameraVector, reflectedLightDirection), 0.0); 
		totalSpecular += (lightColor[i] * pow(specularFactor, 120) * clamp(waterDepth, 0.0, 1.0)) / attenuationFactor; 
	}
		
	vec3 blueWaterColor = vec3(0, 0.2, 0.4); 		
	textureColor = mix(textureColor, blueWaterColor, clamp(waterDepth/100, 0.0, 0.3)); 
		
	float alpha = clamp(waterDepth * 20, 0.0, 1.0); 
	float sunHeightMultiplyer = clamp(unitToLightVector[0].y * 5, 0.0, 1.0);
	pixelColor = vec4(textureColor + min(totalSpecular, sunHeightMultiplyer), alpha);
	
	//pixelColor = texture(refractionTexture, normalizedDeviceCoords + ripple); 
	//pixelColor = texture(reflectionTexture, vec2(normalizedDeviceCoords.x, 1-normalizedDeviceCoords.y) + ripple);
	
}