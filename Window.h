#pragma once

#include <string>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/common.hpp>

class Window {
public:
	Window(const unsigned int windowWidth, const unsigned int windowHeight, const std::string &windowTitle);
	~Window();

	// Clears the window by setting it to a colour
	void clear(const glm::vec3 &colour);

	static inline GLFWwindow* getWindow() { return window; }
private:
	static GLFWwindow *window;

	static friend void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static friend void ResizeCallback(GLFWwindow* window, int width, int height);
};