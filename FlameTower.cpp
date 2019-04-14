#include "FlameTower.h"
#define _USE_MATH_DEFINES
#include <math.h>


FlameTower::FlameTower(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, GLuint bulletTex) :
	TowerObject (entityPos, entityTexture, entityNumElements, bulletTex)
{
	fireRate = 5;
	sep = 0.4f;
	target = nullptr;
	for (int i = 0; i < 4; ++i) {
		bullets.push_back(new BulletObject(position, bulletTexture, entityNumElements));
	}
	
	for (int i = 0; i < bullets.size(); ++i) {
		if (i < 4) {
			bullets[i]->setScale(i * 1.5);
		}
		else {
			bullets[i]->setScale(3 * 1.5);
		}
		bullets[i]->setPosition(glm::vec3(i * cos(glm::radians(orientation)) * sep + position.x,
			i * sin(glm::radians(orientation)) * sep + position.y, 1.0f));
		bullets[i]->setDamage(30);
	}
}


FlameTower::~FlameTower()
{
}

void FlameTower::update(float deltaTime, std::vector<EnemyObject*> targets) {
	if (target == nullptr ||
		(target != nullptr && target->kill) ||
		(target != nullptr && glm::length(glm::vec2(target->getPosition().x - position.x, position.y - target->getPosition().y)) > 3.0f)) {
		float shortest = -1;
		for (int i = 0; i < targets.size(); ++i) {
			float tmp = glm::length(glm::vec2(targets[i]->getPosition().x - position.x, position.y - targets[i]->getPosition().y));
			if (shortest == -1 || tmp < shortest) {
				shortest = tmp;
				target = targets[i];
			}
		}
		isShooting = false;
	}
	// calculate orientation
	if (target != nullptr && glm::length(glm::vec2(target->getPosition().x - position.x, position.y - target->getPosition().y)) < 3.0f) {
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

		isShooting = glm::length(glm::vec2(target->getPosition().x - position.x, position.y - target->getPosition().y)) < 2.5f;
	}
	
	//updateBullets(deltaTime, target);

	if (fireCooldown > 0)
		fireCooldown -= deltaTime;
	else {
		fireCooldown = 1.0f / fireRate;
		//spawn bullet here
		//bullets.push_back(new BulletObject(position, bulletTexture, numElements));
	}
}

void FlameTower::updateBullets(float deltaTime, std::vector<EnemyObject *> targets) {
	bool fireFlag = false;
	for (int i = 0; i < bullets.size(); ++i) {
		bullets[i]->setPosition(glm::vec3(i * cos(glm::radians(orientation)) * sep + position.x,
			i * sin(glm::radians(orientation)) * sep + position.y, 1.0f));
	}
	for (std::vector<BulletObject*>::iterator it = bullets.begin(); it != bullets.end(); ++it) {
		for (int j = 0; j < targets.size(); ++j) {
			if (fireCooldown == 1.0f / fireRate && (*it)->hitsTarget(targets[j])) {
				targets[j]->takeDamage((*it)->getDamage());
				float dist = glm::length(glm::vec2(targets[j]->getPosition().x - position.x, position.y - targets[j]->getPosition().y));
				if (dist < 1.5f) {
					targets[j]->takeDamage((*it)->getDamage());
				}
			}
		}
	}
}


void FlameTower::render(Shader &shader) {
	GameObject::render(shader);
}
