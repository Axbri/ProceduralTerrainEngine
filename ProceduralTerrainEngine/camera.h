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
	void setAspectRatio();
	void update(double delta_time);
	void setPosition(Vec3 pos); 
	void setTargetPosition(Vec3 pos);
	void pan(double amount); 
	void tilt(double amount);
	void setPan(double angle);
	void setTilt(double angle);
	void setTargetPan(double angle);
	void setTargetTilt(double angle);
	void moveYpos(double distance); 
	void invertTilt(); 
	Mat4 getViewMatrix();
	Mat4 getProjectionMatrix();
	Vec3 getPosition(); 
	Vec3 getViewVector(); 

	const double NEAR_CLIP = 0.01;
	const double FAR_CLIP = 200;
	const double SMOOTH_POSITION_FOLLOW = 30;
	const double SMOOTH_DIRECTION_FOLLOW = 20;
	
private:
	void updateViewMatrix(); 

	const double HEAD_OFFSET = 0.5;

	Vec3 position;
	double panAngle;
	double tiltAngle;

	Vec3 targetPosition;
	double targetPanAngle;
	double targetTiltAngle;

	double aspectRatio; 
	Mat4 viewMatrix;
	Mat4 projMatrix;		
};