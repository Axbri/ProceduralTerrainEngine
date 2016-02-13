#pragma once

#include <GL/glew.h> 
#include <algorithm>
#include "userinput.h"
#include "terrainheightgenerator.h"
#include "mat4.h"

using namespace std;

class Camera
{
public:
	Camera();
	void setAspectRatio(double aspectRatio);
	void update(double delta_time);
	void setPosition(Vec3 pos); 
	void pan(double amount); 
	void tilt(double amount);
	void setPan(double angle);
	void setTilt(double angle);
	Mat4 getViewMatrix();
	Mat4 getProjectionMatrix();
	Vec3 getPosition(); 
	Vec3 getViewVector(); 

	const double NEAR_CLIP = 0.01;
	const double FAR_CLIP = 100;

private:
	void updateViewMatrix(); 
	Vec3 position;
	double panAngle;
	double tiltAngle;
	double aspectRatio; 
	Mat4 viewMatrix;
	Mat4 projMatrix;
	
	const double HEAD_OFFSET = 0.5;
	
	
};