#pragma once

#include "vec3.h"

class Settings
{
public:
	Settings();

	float getFogDencity(); 	
	Vec3 getFogColor(); 
	float getGamma();

private: 
	float fogDencity; 
	Vec3 fogColor; 
	float gamma;
};

