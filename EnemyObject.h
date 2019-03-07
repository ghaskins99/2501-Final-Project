#pragma once

#include "GameObject.h"
#include "Node.h"

// Inherits from GameObject
class EnemyObject : public GameObject {
public:
	EnemyObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, float s);

	// Update function for moving the player object around
	virtual void update(double deltaTime) override;
	virtual void render(Shader &shader) override;

	void setPath(std::vector<Node*> p);

	void nextNode();

	inline Node* getCurrNode() { return currNode; }
	inline bool getFinished() { return finished; }

protected:
	// current path to follow
	std::vector<Node*> path;
	// current "target" node we are heading towards
	Node * currNode;

	// orientation, in degrees
	float orientation;

	// if we have finished traversing our current path
	bool finished;

	// speed travelled at
	float speed;
};
