#include "BulletObject.h"
#include <GL/glfw3.h>

#include "Window.h"
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

/*
	BulletObject inherits from GameObject
*/

BulletObject::BulletObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements)
	: GameObject(entityPos, entityTexture, entityNumElements, true) {
	orientation = 0.0f;
	speed = 10.0f;
	erase = false;
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

bool BulletObject::hitsTarget(GameObject* target) {
	float dx = position.x - target->getPosition().x;
	float dy = position.y - target->getPosition().y;

	float distance = sqrt(pow(dx, 2) + pow(dy, 2));

	if (distance < (target->getScale() / 2) + (scale / 20)) {
		return true;
	}
	return false;
}