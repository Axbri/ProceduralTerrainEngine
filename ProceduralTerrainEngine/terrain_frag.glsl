#version 330 core

in vec3 interpolatedCameraSpacePos; 
in vec3 interpolatedWorldPos; 
in vec2 interpolatedTextureCoords; 
in vec3 interpolatedNormal;
in vec3 toLightVector[8]; 
in vec3 toCameraVector; 

out vec4 pixel_color; 

uniform sampler2D grassTexture; 
uniform sampler2D rockTexture; 
uniform sampler2D sandTexture; 

uniform vec3 lightColor[8]; 
uniform vec3 lightAttenuation[8];
uniform vec3 fogColor;
uniform float fogDencity; 
uniform float gamma;

const float shineDampener = 0.0; 
const float reflectance = 0.0; 
const float ambient = 0.15; 

void main (void) 
{
	vec3 normal = normalize(interpolatedNormal); 	

	vec4 grassColor = texture(grassTexture, interpolatedTextureCoords * 32);
	vec4 rockColor = texture(rockTexture, interpolatedTextureCoords * 16);
	vec4 sandColor = texture(sandTexture, interpolatedTextureCoords * 16);
	
	float normalAngle = clamp(length(vec2(normal.x, normal.z)), 0, 1); 
	
	float sandMixFactor = normalAngle/8 + clamp(1-interpolatedWorldPos.y/2, 0, 1); 	
	
	vec4 textureColor = mix(grassColor, sandColor, smoothstep(0.4, 0.9, sandMixFactor)); 
	
	float rockMixFactor = smoothstep(0.12, 0.30, normalAngle); 		
	textureColor = mix(textureColor, rockColor, rockMixFactor); 
						
			
	vec3 unitToCameraVector = normalize(toCameraVector);	
	vec3 totalDiffuse = vec3(0.0); 
	vec3 totalSpecular = vec3(0.0); 
	
	for (int i = 0; i < 8; i++) {
		float distanceToLight = length(toLightVector[i]); 
		float attenuationFactor = lightAttenuation[i].x + (lightAttenuation[i].y * distanceToLight) + (lightAttenuation[i].z * distanceToLight * distanceToLight);
		vec3 unitToLightVector = normalize(toLightVector[i]); 		
		float nDot1 = dot(normal, unitToLightVector); 
		float brightness = max(nDot1, 0.0); 		
		vec3 lightDirection = -unitToLightVector; 	
		vec3 reclectedLightDirection = reflect(lightDirection, normal); 	
		float specularFactor = max(dot(unitToCameraVector, reclectedLightDirection), 0.0); 
		totalDiffuse = totalDiffuse + (brightness * lightColor[i]) / attenuationFactor; 
		totalSpecular = totalSpecular + (pow(specularFactor, shineDampener) * lightColor[i] * clamp(reflectance, 0.0, 1.0)) / attenuationFactor; 
	}
		
	totalDiffuse = max(totalDiffuse, ambient); 	// ambient light
	totalDiffuse = min(totalDiffuse, 1.0);
	totalSpecular = min(totalSpecular, 1.0);
	
	vec3 materialAndLighting = textureColor.xyz * totalDiffuse + totalSpecular; 
	//pixel_color = vec4(materialAndLighting, 1.0);	
	
	float toFragmentDistance = length(interpolatedCameraSpacePos);     	
	float fogMultiplier = clamp(-0.2 + toFragmentDistance * fogDencity, 0.0, 1.0); 

	vec3 finalColor = mix(materialAndLighting, fogColor, fogMultiplier);		
	pixel_color = vec4(pow(materialAndLighting, vec3(1.0 / gamma)), smoothstep(0.0, 0.2, 1-fogMultiplier));	
}