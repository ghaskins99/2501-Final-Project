#include "Renderable.h"

Renderable::Renderable(glm::vec3 &entityPosition, GLuint entityTexture, GLint entityNumElements)
{
	position = entityPosition;
	texture = entityTexture;
	numElements = entityNumElements;
	scale = glm::vec3(1.0f);
}

void Renderable::render(Shader &shader) {
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader
	// TODO: Add different types of transformations
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(), glm::vec3(0.2f, 0.3f, 1.0f) * scale);

	glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	shader.setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}