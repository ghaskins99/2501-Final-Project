#include "SlowTower.h"

SlowTower::SlowTower(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, GLuint bulletTex) :
	TowerObject(entityPos, entityTexture, entityNumElements, bulletTex)
{
	range = 2.0f;
}


SlowTower::~SlowTower()
{
}

void SlowTower::update(float deltaTime, std::vector<EnemyObject *> targets) {
	for (int i = 0; i < targets.size(); ++i) {
		if (std::find(targetsInRange.begin(), targetsInRange.end(), targets[i]) == targetsInRange.end()
			&& glm::length(targets[i]->getPosition() - position) <= range) {
			targets[i]->speedMultiplier *= 0.5;
			targetsInRange.push_back(targets[i]);
		}
	}

	for (std::vector<EnemyObject*>::iterator it = targetsInRange.begin(); it != targetsInRange.end();) {
		if ((*it)->kill || glm::length(position - (*it)->getPosition()) > range) {
			(*it)->speedMultiplier /= 0.5;
			it = targetsInRange.erase(it);
		}
		else {
			++it;
		}
	}
}

void SlowTower::render(Shader &shader) {
	GameObject::render(shader);
}
