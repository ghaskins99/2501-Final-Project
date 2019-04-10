#pragma once

#include <GL/glew.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "FileUtils.h"

class Shader {
public:
	Shader(const char *vertPath, const char *fragPath);
	Shader() {};
	~Shader();

	virtual void attributeBinding();

	void enable();
	void disable();

	// Sets a uniform integer variable in your shader program to a value
	void setUniform1i(const GLchar *name, int value);

	// Sets a uniform float variable in your shader program to a value
	void setUniform1f(const GLchar *name, float value);

	// Sets a uniform vector2 variable in your shader program to a vector
	void setUniform2f(const GLchar *name, const glm::vec2 &vector);

	// Sets a uniform vector3 variable in your shader program to a vector
	void setUniform3f(const GLchar *name, const glm::vec3 &vector);

	// Sets a uniform vector4 variable in your shader program to a vector
	void setUniform4f(const GLchar *name, const glm::vec4 &vector);

	// Sets a uniform matrix4x4 variable in your shader program to a matrix4x4
	void setUniformMat4(const GLchar *name, const glm::mat4 &matrix);

	// Getters
	inline GLuint getShaderID() { return shaderProgram; }
private:
	GLuint shaderProgram;
};