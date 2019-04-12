#pragma once

#include<vector>
#include "Renderable.h"
#include "UIElement.h"

class AmountView : public UIElement {
public:
	AmountView(GLuint* f, glm::vec3 p, GLint s, int, glm::vec3);
	void add(int);
	virtual void render(Shader &shader);

	inline int getAmount() { return amount; }
	inline glm::vec3 getPosition() { return position; }
private:
	int amount;
	std::vector<Renderable*> view;
	glm::vec3 colorMod;
};

