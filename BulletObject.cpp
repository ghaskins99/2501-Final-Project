#include "BulletObject.h"
#include <GL/glfw3.h>

#include "Window.h"
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

/*
	BulletObject inherits from GameObject
*/

BulletObject::BulletObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, float entityOrientation, bool n)
	: GameObject(entityPos, entityTexture, entityNumElements, n) {
	orientation = entityOrientation;
	speed = 10.0f;
	erase = false;

	// so that it spawns near the front of the ship instead of directly in the center
	position.x += speed * (float)cos(glm::radians(orientation)) * 0.025f;
	position.y += speed * (float)sin(glm::radians(orientation)) * 0.025f;
}

void BulletObject::update(double deltaTime, GameObject* target) {
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

	GameObject::update(deltaTime);
}