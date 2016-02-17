
#include "camera.h"

using namespace std;

Camera::Camera()
{
	position = Vec3{ 0.0, 0.0, 0.0 };
	panAngle = 0; 
	tiltAngle = 0;
	targetPosition = Vec3{ 0.0, 0.0, 0.0 };
	targetPanAngle = 0;
	targetTiltAngle = 0;
	updateViewMatrix();
	projMatrix.loadPerspectiveProjection(2, 1.2f, NEAR_CLIP, FAR_CLIP);	
}

void Camera::setAspectRatio()
{
	double aspectRatio = WindowSizeHandler::getFrameBufferSize().y / WindowSizeHandler::getFrameBufferSize().x;
	projMatrix.loadPerspectiveProjection(aspectRatio, 1.2f, NEAR_CLIP, FAR_CLIP);
}

// Update the cameras position, call this function in the main update loop.
void Camera::update(double delta_time)
{
	// smoothly move the camera around according to the target values. 
	double posMultiplyer = min(SMOOTH_POSITION_FOLLOW * delta_time, 1.0); 
	double dirMultiplyer = min(SMOOTH_DIRECTION_FOLLOW * delta_time, 1.0);
	position += (targetPosition - position) * posMultiplyer;
	panAngle += (targetPanAngle - panAngle) * dirMultiplyer;
	tiltAngle += (targetTiltAngle - tiltAngle) * dirMultiplyer;

	updateViewMatrix(); 
}

void Camera::setPosition(Vec3 pos)
{
	this->position = pos; 
	this->targetPosition = pos;
}

void Camera::setTargetPosition(Vec3 pos)
{
	this->targetPosition = pos;
}

void Camera::pan(double amount)
{
	targetPanAngle += amount;
}

void Camera::tilt(double amount)
{
	targetTiltAngle += amount;
}

void Camera::setPan(double angle)
{
	this->panAngle = angle; 
	this->targetPanAngle = angle; 
}

void Camera::setTilt(double angle)
{
	this->tiltAngle = angle; 
	this->targetTiltAngle = angle; 
}

void Camera::setTargetPan(double angle)
{
	this->targetPanAngle = angle;
}

void Camera::setTargetTilt(double angle)
{
	this->targetTiltAngle = angle;
}

void Camera::moveYpos(double distance)
{
	this->position.y += distance;
	updateViewMatrix();
}

void Camera::invertTilt()
{
	this->tiltAngle = -tiltAngle;
	updateViewMatrix();
}

Mat4 Camera::getViewMatrix()
{
	return viewMatrix; 
}

Mat4 Camera::getProjectionMatrix()
{
	return projMatrix; 
}

Vec3 Camera::getPosition()
{
	return Vec3{ position };
}

Vec3 Camera::getViewVector()
{
	return Vec3{ -sin(panAngle) * cos(tiltAngle), sin(tiltAngle), -cos(panAngle) * cos(tiltAngle) };
}

// This function also updates the camera's view matrix using 
// the variables describing it's position and rotation.
void Camera::updateViewMatrix()
{	
	Mat4 mainTranslation, orbitRotation, tiltRotation, distanceTranslation;

	mainTranslation.loadTranslation((float)-position.x, (float)-position.y, (float)-position.z);
	orbitRotation.loadRotationY((float)panAngle);
	tiltRotation.loadRotationX((float)tiltAngle);
	distanceTranslation.loadTranslation(0, 0, (float)HEAD_OFFSET);

	viewMatrix = mainTranslation * orbitRotation * tiltRotation * distanceTranslation; 
}