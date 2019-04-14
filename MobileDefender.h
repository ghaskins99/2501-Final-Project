#pragma once
#include "GameObject.h"
#include "EnemyObject.h"
#include "TowerObject.h"
#include "Node.h"

class MobileDefender : public TowerObject
{
public:
	MobileDefender(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, GLuint bulletTex);
	~MobileDefender();

	// Update function for moving the player object around
	virtual void update(float deltaTime, GameObject* target) override;
	virtual void render(Shader &shader) override;

	void setPath(std::vector<Node*> p);

	void nextNode();

	inline Node* getCurrNode() { return currNode; }
	inline bool getFinished() { return finished; }

	inline virtual std::string getType() { return "mobile"; }

protected:
	// current path to follow
	std::vector<Node*> path;
	// current "target" node we are heading towards
	Node * currNode;
	glm::vec3 velocity;

	// if we have finished traversing our current path
	bool finished;
	float moveOrientation;

	float range;
	float a_max;
	
};

