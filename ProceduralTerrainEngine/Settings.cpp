#include "Settings.h"



Settings::Settings()
{
	this->fogDencity = 0.007f; 
	this->fogColor = Vec3{ 0.45, 0.60, 0.70 };
	this->gamma = 1.2f; 
}

float Settings::getFogDencity()
{
	return fogDencity;
}

Vec3 Settings::getFogColor()
{
	return fogColor;
}

float Settings::getGamma()
{
	return gamma;
}