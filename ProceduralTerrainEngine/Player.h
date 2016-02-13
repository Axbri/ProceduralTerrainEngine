#pragma once

#include "vec3.h"
#include "camera.h"

class Player
{
public:
	Player(double x, double z, double aspectRatio);

	void update(GLFWwindow* window, double deltaTime);
	Camera getCamera(); 

private: 

	void forceForward(double amount);
	void forceRight(double amount);
	void forceUp(double amount);
	void impulseUp(double amount);
	void doPhysics(double deltaTime); 

	const double MOUSE_ROTATION_SENSITIVITY = 0.015;
	const double EYE_HEIGHT = 1.3;
	const double AIR_FRICTION = 0.15;
	const double GROUND_FRICTION = 0.10;
	const double WALKING_SPEED = 0.4;
	const double JUMP_SPEED = 0.025;
	const double RUNNING_MULTIPLIER = 1.7;
	const double GRAVITY = -0.09;

	Camera camera;

	Vec3 position; 
	Vec3 speed;

	double facingDirection;
	double headTiltAngle; 		
	double xMomentum;
	double zMomentum;
	double yMomentum;
	bool inAir; 
	double friction;	
};

