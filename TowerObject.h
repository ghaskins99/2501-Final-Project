#pragma once

#include "GameObject.h"
#include "BulletObject.h"
#include <vector>

// Inherits from GameObject
class TowerObject : public GameObject {
public:
	TowerObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, GLuint bulletTex);

	// Update function for moving the player object around
	virtual void update(float deltaTime, GameObject *target);
	virtual void render(Shader &shader);
	void updateBullets(float deltaTime, GameObject *target);
protected:
	std::vector<BulletObject*> bullets;
	GLuint bulletTexture;
	float fireRate;
	float fireCooldown;
};
