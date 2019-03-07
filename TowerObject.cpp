#include "TowerObject.h"
#include <GL/glfw3.h>

#include "Window.h"
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

/*
	TowerObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

TowerObject::TowerObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, bool n)
	: GameObject(entityPos, entityTexture, entityNumElements, n) {
	orientation = 90.0f;
	erase = false;
	rotationSpeed = 0.0f;
	speed = 0.0f;
}

// Update function for moving the player object around
void TowerObject::update(GameObject* target) {
	// calculate orientation
	float dx = target->getPosition().x - position.x;
	float dy = position.y - target->getPosition().y;

	float radians = atan2f(dy, dx);

	if (radians < 0) {
		radians = abs(radians);
	}
	else {
		radians = 2 * M_PI - radians;
	}

	float degrees = glm::degrees(radians);

	orientation = degrees;
}
