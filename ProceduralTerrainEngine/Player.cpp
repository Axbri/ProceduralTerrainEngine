#include "Player.h"

Player::Player(double x, double z)
{
	position = Vec3{ 0, 0, 0 };
	facingDirection = headTiltAngle = 0;
	camera.setAspectRatio(); 
	xMomentum = zMomentum = yMomentum = 0;
	inAir = false; 
}

void Player::update(GLFWwindow* window, double deltaTime)
{
	double leftRight{ 0 }; 
	double forwardBackward{ 0 }; 

	if (UserInput::pollKey(window, GLFW_KEY_A))
		leftRight = -1;
	if (UserInput::pollKey(window, GLFW_KEY_D))
		leftRight = 1;
	if (UserInput::pollKey(window, GLFW_KEY_S))
		forwardBackward = -1;
	if (UserInput::pollKey(window, GLFW_KEY_W))
		forwardBackward = 1;

	if (UserInput::pollKey(window, GLFW_KEY_SPACE)) {
		if (!inAir) {
			impulseUp(JUMP_SPEED, deltaTime);
		}
	}

	double speed = WALKING_SPEED;
	forceForward(forwardBackward * speed);
	forceRight(leftRight * speed);

	doPhysics(deltaTime); 

	if (UserInput::isCursorLocked()) {
		Vec2 mouseVel = UserInput::getMouseVel();
		facingDirection -= mouseVel.x * MOUSE_ROTATION_SENSITIVITY;
		headTiltAngle -= mouseVel.y * MOUSE_ROTATION_SENSITIVITY;
		headTiltAngle = max(-3.14 / 2, min(headTiltAngle, 3.14 / 2));
	}	

	camera.setTargetPosition(Vec3(position.x, position.y + EYE_HEIGHT, position.z));
	camera.setTargetPan(facingDirection);
	camera.setTargetTilt(headTiltAngle);
	camera.update(deltaTime);
}

Camera Player::getCamera()
{
	return camera;
}

Vec3 Player::getPosition()
{
	return position;
}

void Player::forceForward(double amount)
{
	xMomentum -= amount * sin(facingDirection);
	zMomentum -= amount * cos(facingDirection);
}

void Player::forceRight(double amount)
{
	xMomentum += amount * cos(facingDirection);
	zMomentum -= amount * sin(facingDirection);
}

void Player::forceUp(double amount)
{
	yMomentum += amount;
}

void Player::impulseUp(double amount, double deltaTime)
{
	speed.y += amount; // *deltaTime;
	if (amount > 0) {
		inAir = true;
	}
}

void Player::doPhysics(double deltaTime)
{
	if (inAir)
		friction = AIR_FRICTION;
	else
		friction = GROUND_FRICTION;

	xMomentum *= (1 - friction);
	yMomentum *= (1 - friction);
	zMomentum *= (1 - friction);

	speed.x = xMomentum * deltaTime;		
	speed.y += GRAVITY * deltaTime;	
	speed.z = zMomentum * deltaTime;

	position += speed;

	float terrainHeight = TerrainHeightGenerator::getHeight(position.x, position.z);

	if (position.y < terrainHeight + 0.02f) {
		position.y = terrainHeight;
		inAir = false;
		speed.y = 0;
	}
}
