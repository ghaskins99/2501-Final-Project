#pragma once

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>

#include "Shader.h"

class Renderable
{
public:
	Renderable(glm::vec3 &entityPosition, GLuint entityTexture, GLint entityNumElements);
	virtual void render(Shader &shader);

	inline void setTex(GLuint t) { texture = t; }
	inline GLuint getTex() { return texture; }
	inline void setScale(glm::vec3 s) { scale = s; }
protected:
	glm::vec3 position;
	GLint numElements;
	GLuint texture;
private:
	glm::vec3 scale;
};

