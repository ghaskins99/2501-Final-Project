#pragma once

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>

#include "Shader.h"


class GameObject {
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
	inline bool getErase() { return erase; }
	inline void setErase(bool b) { erase = b; }

	// Setters
	inline void setPosition(glm::vec3& newPosition) { position = newPosition; }
protected:
	// Object's Transform Variables
	// TODO: Add more transformation variables
	glm::vec3 position;
	float orientation;
	float speed;
	float scale; // this is my object size because i didnt realize you made a variable for it already
	float rotationSpeed;

	// Object's details
	GLint numElements;
	float objectSize; // Not currently being used (will be needed for collision detection when objects have a different scale)

	// Object's texture
	GLuint texture;
	bool erase;
	bool normalObj;
};