#pragma once

#include "Renderable.h"

class UIElement {
public:
	UIElement(GLuint* f, glm::vec3 p, GLint s);
	virtual void render(Shader&) = 0;
protected:
	GLuint* font;
	glm::vec3 position;
	GLint numElements;
};

