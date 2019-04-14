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

TowerObject::TowerObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, GLuint bulletTex)
	: GameObject(entityPos, entityTexture, entityNumElements, true) {
	orientation = 90.0f;
	rotationSpeed = 0.0f;
	speed = 0.0f;
	bulletTexture = bulletTex;
	
	fireCooldown = 0;
	fireRate = 1.5f;
	//spawn a bullet
	bullets.push_back(new BulletObject(position, bulletTexture, entityNumElements));
}

// Update function for moving the player object around
void TowerObject::update(float deltaTime, GameObject* target) {
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
	updateBullets(deltaTime, target);

	if (fireCooldown > 0)
		fireCooldown -= deltaTime;
	else {
		fireCooldown = 1.0f / fireRate;
		//spawn bullet here
		bullets.push_back(new BulletObject(position, bulletTexture, numElements));
	}
}

void TowerObject::updateBullets(float deltaTime, GameObject* target) {
	// removing elements from the game array while iterating, don't think could do in "for i=x" loop
	for (std::vector<BulletObject*>::iterator it = bullets.begin(); it != bullets.end();) {
		(*it)->update(deltaTime, target);

		if ((*it)->hitsTarget(target)) {
			target->takeDamage((*it)->getDamage());
			delete (*it);
			it = bullets.erase(it);
		}
		else {
			++it;
		}
	}
}

void TowerObject::render(Shader &shader) {
	GameObject::render(shader);
	for (BulletObject* b : bullets) {
		b->render(shader);
	}
}