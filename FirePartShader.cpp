#include "FirePartShader.h"

FirePartShader::FirePartShader() {
	// Set up shaders

	// Create a shader from vertex program source code
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &source_vpart, NULL);
	glCompileShader(vs);

	// Check if shader compiled successfully
	GLint status;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(vs, 512, NULL, buffer);
		throw(std::ios_base::failure(std::string("Error compiling vertex shader: ") + std::string(buffer)));
	}

	// Create a shader from the fragment program source code
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &source_fp, NULL);
	glCompileShader(fs);

	// Check if shader compiled successfully
	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(fs, 512, NULL, buffer);
		throw(std::ios_base::failure(std::string("Error compiling fragment shader: ") + std::string(buffer)));
	}

	// Create a shader program linking both vertex and fragment shaders
	// together
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);

	// Check if shaders were linked successfully
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(shaderProgram, 512, NULL, buffer);
		throw(std::ios_base::failure(std::string("Error linking shaders: ") + std::string(buffer)));
	}

	// Delete memory used by shaders, since they were already compiled
	// and linked
	glDeleteShader(vs);
	glDeleteShader(fs);

	attributeBinding();
}

void FirePartShader::attributeBinding() {
	// Set attributes for shaders
	// Should be consistent with how we created the buffers for the particle elements
	GLint vertex_att = glGetAttribLocation(shaderProgram, "vertex");
	glVertexAttribPointer(vertex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(vertex_att);

	GLint dir_att = glGetAttribLocation(shaderProgram, "dir");
	glVertexAttribPointer(dir_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(dir_att);

	GLint time_att = glGetAttribLocation(shaderProgram, "t");
	glVertexAttribPointer(time_att, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(4 * sizeof(GLfloat)));
	glEnableVertexAttribArray(time_att);

	GLint tex_att = glGetAttribLocation(shaderProgram, "uv");
	glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(tex_att);
}

FirePartShader::~FirePartShader() {
	glDeleteProgram(shaderProgram);
}