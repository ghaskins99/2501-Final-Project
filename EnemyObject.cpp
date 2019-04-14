#include "EnemyObject.h"

#include "Window.h"
#define _USE_MATH_DEFINES
#include <math.h>

/*
	EnemyObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

EnemyObject::EnemyObject(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, float s)
	: GameObject(entityPos, entityTexture, entityNumElements) {
	orientation = 0.0f;
	finished = true;
	speed = s;
	hp = 500.0f;
	currNode = nullptr;
}

// Update function for moving the player object around
void EnemyObject::update(double deltaTime) {
	if (finished || kill) {
		return;
	}

	// calculate orientation 
	float dx = currNode->getX() - position.x;
	float dy = position.y - currNode->getY();

	float radians = atan2f(dy, dx);

	if (radians < 0) {
		radians = abs(radians);
	}
	else {
		radians = 2 * M_PI - radians;
	}

	float degrees = glm::degrees(radians);

	orientation = degrees;

	position.x += speed * (float)deltaTime * cos(glm::radians(orientation));
	position.y += speed * (float)deltaTime * sin(glm::radians(orientation));
}

// slightly modified render function so the players appear larger
void EnemyObject::render(Shader &shader) {
	if (kill) return;

	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.4f, 0.58f, 0.0f));


	// Set the transformation matrix in the shader
	glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	//transformationMatrix = rotationMatrix * translationMatrix  * scaleMatrix;
	shader.setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}

// reset the player's path to the parameter
void EnemyObject::setPath(std::vector<Node*> p) {
	finished = false;

	path = p;

	currNode = path.back();
}

// set current "target" node to the next one specified in the path, pop the previous node off
void EnemyObject::nextNode() {
	path.pop_back();

	if (path.empty()) {
		finished = true;
		return;
	}

	currNode = path.back();
}