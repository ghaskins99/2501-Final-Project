#include "GameObject.h"

/*
	GameObject is responsible for handling the rendering and updating of objects in the game world
	The update method is virtual, so you can inherit from GameObject and override the update functionality (see PlayerGameObject for reference)
*/

// used and extended by bullet, player, shield
GameObject::GameObject(glm::vec3 &entityPosition, GLuint entityTexture, GLint entityNumElements) {
	position = entityPosition;
	texture = entityTexture;
	numElements = entityNumElements;
	orientation = 0.0f; // so many default values... just to be safe
	speed = 0.0f;
	erase = false;
	scale = 1.0f;
	rotationSpeed = 0.0f;
	normalObj = false;
	kill = false;
}

// this should only be used by asteroids/targets
GameObject::GameObject(glm::vec3 &entityPosition, GLuint entityTexture, GLint entityNumElements, bool normal)
	: GameObject(entityPosition, entityTexture, entityNumElements) {
	normalObj = normal;
}

// Updates the GameObject's state. Can be overriden for children
void GameObject::update(double deltaTime) {
	if (kill) return;

	// Update object position
	//position += velocity * (float)deltaTime;

	position.x += speed * (float)deltaTime * cos(glm::radians(orientation));
	position.y += speed * (float)deltaTime * sin(glm::radians(orientation));
}

// Renders the GameObject using a shader. can be overridden
void GameObject::render(Shader &shader) {
	if (kill) return;

	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader
	// TODO: Add different types of transformations
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), orientation, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMatrix;
	if (normalObj) {
		scaleMatrix = glm::scale(glm::mat4(), glm::vec3(scale, scale, scale));
	}
	else {
		scaleMatrix = glm::scale(glm::mat4(), glm::vec3(0.2f, 0.3f, 0.3f));
	}

	// Set the transformation matrix in the shader
	// TODO: Multiply your new transformations to make the transformationMatrix
	glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	shader.setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}

void GameObject::takeDamage(float damage) {
	hp -= damage;
	if (hp <= 0.0f) kill = true;
}