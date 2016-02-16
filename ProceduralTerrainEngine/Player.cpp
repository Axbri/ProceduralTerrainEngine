#include "Player.h"

Player::Player(double x, double z)
{
	position = Vec3{ 0, 0, 0 };
	facingDirection = headTiltAngle = 0;
	camera.setAspectRatio(); 
	momentum = Vec3{ 0, 0, 0 };
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

	//if (UserInput::pollKey(window, GLFW_KEY_SPACE)) {
	//	if (!inAir) {
	//		impulseUp(JUMP_SPEED);
	//	}
	//}

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

Vec3 Player::getVelocity()
{
	return velocity;
}

void Player::forceForward(double amount)
{
	momentum.x -= amount * sin(facingDirection);
	momentum.z -= amount * cos(facingDirection);
}

void Player::forceRight(double amount)
{
	momentum.x += amount * cos(facingDirection);
	momentum.z -= amount * sin(facingDirection);
}

void Player::forceUp(double amount)
{
	momentum.y += amount;
}

void Player::impulseUp(double amount)
{
	velocity.y += amount;
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
	
	momentum.x *= (1 - friction);
	momentum.y *= (1 - friction);
	momentum.z *= (1 - friction);

	velocity.x = momentum.x * deltaTime;
	velocity.y += GRAVITY * deltaTime;
	velocity.z = momentum.z * deltaTime;

	position += velocity;

	float terrainHeight = 10; // TerrainHeightGenerator::getHeight(position.x, position.z);

	if (position.y < terrainHeight + 0.02f) {
		position.y = terrainHeight;
		inAir = false;
		velocity.y = 0;
	}
}
