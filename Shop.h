#pragma once
#include <vector>
#include "UIElement.h"
#include "AmountView.h"

class Shop : public UIElement {
public:
	Shop(GLuint* f, glm::vec3 p, GLint s, std::vector<GLuint> t);
	virtual void render(Shader &shader);
private:
	std::vector<Renderable*> items;
	std::vector<AmountView*> costs;
};

