#pragma once

#include "GameObject.h"

// Inherits from GameObject
class BulletObject : public GameObject {
public:
	BulletObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, float entityOrientation, bool n);
	void update(double deltaTime, GameObject* target);
};
