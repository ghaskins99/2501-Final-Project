#pragma once

#include "Renderable.h"

class GameObject : public Renderable {
public:
	GameObject(glm::vec3 &entityPosition, GLuint entityTexture, GLint entityNumElements);
	GameObject(glm::vec3 &entityPosition, GLuint entityTexture, GLint entityNumElements, bool normal);

	// Updates the GameObject's state. Can be overriden for children
	virtual void update(double deltaTime);

	// Renders the GameObject using a shader. Override for playerObj
	virtual void render(Shader &shader);


	// Getters
	inline glm::vec3& getPosition() { return position; }
	inline float& getOrientation() { return orientation; }
	inline float& getSpeed() { return speed; }
	inline float& getScale() { return scale; }

	// Setters
	inline void setPosition(glm::vec3& newPosition) { position = newPosition; }
	virtual void takeDamage(float);

	bool kill;
protected:
	// Object's Transform Variables
	// TODO: Add more transformation variables
	float orientation;
	float speed;
	float scale; // this is my object size because i didnt realize you made a variable for it already
	float rotationSpeed;
	float hp;

	// Object's details
	float objectSize; // Not currently being used (will be needed for collision detection when objects have a different scale)

	bool normalObj;
};