#pragma once

#include "GameObject.h"
#include <vector>

// Inherits from GameObject
class TowerObject : public GameObject {
public:
	TowerObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, bool n);

	// Update function for moving the player object around
	void update(GameObject *target);
};
