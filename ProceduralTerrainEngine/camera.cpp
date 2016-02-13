
#include "camera.h"

using namespace std;

Camera::Camera()
{
	position = Vec3{ 0.0, 0.0, 0.0};
	panAngle = tiltAngle = 0;
	updateViewMatrix();
	projMatrix.loadPerspectiveProjection(2, 1.2f, NEAR_CLIP, FAR_CLIP);
	
}

void Camera::setAspectRatio(double aspectRatio)
{
	this->aspectRatio = aspectRatio; 
	projMatrix.loadPerspectiveProjection(aspectRatio, 1.2f, NEAR_CLIP, FAR_CLIP);
}

// Update the cameras position, call this function in the main update loop.
void Camera::update(double delta_time)
{
	updateViewMatrix(); 
}

void Camera::setPosition(Vec3 pos)
{
	this->position = pos; 
}

void Camera::pan(double amount)
{
	panAngle += amount; 
}

void Camera::tilt(double amount)
{
	tiltAngle += amount; 
}

void Camera::setPan(double angle)
{
	this->panAngle = angle; 
}

void Camera::setTilt(double angle)
{
	this->tiltAngle = angle; 
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