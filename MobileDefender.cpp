#include "MobileDefender.h"
#define _USE_MATH_DEFINES
#include <math.h>


MobileDefender::MobileDefender(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, GLuint bulletTex)
	: TowerObject(entityPos, entityTexture, entityNumElements, bulletTex) {
	orientation = 0.0f;
	finished = true;
	speed = 4.f;
	hp = 500.0f;
	range = 1.5f;
	fireRate = 3.0f;
	damage = 50.f;
}


MobileDefender::~MobileDefender()
{
}

void MobileDefender::update(float deltaTime, GameObject* target) {
	if (finished || kill) {
		return;
	}

	if (glm::length(target->getPosition() - position) <= range) {
		finished = true;
		path.clear();
		TowerObject::update(deltaTime, target);
	}
	else {
		if (!finished && glm::length(position - glm::vec3(currNode->getX(), currNode->getY(), 0.0f)) < 0.05) {
			nextNode();
		}
		if (fireCooldown > 0)
			fireCooldown -= deltaTime;

		// calculate movement orientation 
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

		moveOrientation = degrees;

		position.x += speed * (float)deltaTime * cos(glm::radians(moveOrientation));
		position.y += speed * (float)deltaTime * sin(glm::radians(moveOrientation));

		updateBullets(deltaTime, target);

		// calculate look orientation
		dx = target->getPosition().x - position.x;
		dy = position.y - target->getPosition().y;

		radians = atan2f(dy, dx);

		if (radians < 0) {
			radians = abs(radians);
		}
		else {
			radians = 2 * M_PI - radians;
		}

		degrees = glm::degrees(radians);

		orientation = degrees;
	}
}

// slightly modified render function so the players appear larger
void MobileDefender::render(Shader &shader) {
	if (kill) return;

	//// Bind the entities texture
	//glBindTexture(GL_TEXTURE_2D, texture);

	/*// Setup the transformation matrix for the shader
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.4f, 0.58f, 0.0f));


	// Set the transformation matrix in the shader
	glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	//transformationMatrix = rotationMatrix * translationMatrix  * scaleMatrix;
	shader.setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);*/

	TowerObject::render(shader);
}

// reset the player's path to the parameter
void MobileDefender::setPath(std::vector<Node*> p) {
	finished = false;

	path = p;

	currNode = path.back();
}

// set current "target" node to the next one specified in the path, pop the previous node off
void MobileDefender::nextNode() {
	path.pop_back();

	if (path.empty()) {
		finished = true;
		return;
	}

	currNode = path.back();
}
