#pragma once

#include "GameObject.h"

// Inherits from GameObject
class BulletObject : public GameObject {
public:
	BulletObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements);
	void update(double deltaTime, GameObject* target);
	bool hitsTarget(GameObject* target);
	
	inline float getDamage() { return damage; }

private:
	float damage;
};
