#pragma once

#include <vector>
#include <string>

#include "Renderable.h"
#include "AmountView.h"
#include "Shop.h"

class UIController
{
public:
	UIController(GLuint*, GLint, std::vector<GLuint>);
	void render(Shader &shader);

	void changeMoney(int c);
	void changeHealth(int c);
	int getShopCost(int num);

	inline int getCurrentMoney() { return money->getAmount(); }
private:
	GLuint* fakefont;
	GLint numElements;
	glm::vec3 startPos = glm::vec3(3.2f, 4.7f, 1.0f);

	Renderable* backdrop;
	Renderable* cancel;
	AmountView* money;
	AmountView* health;
	Shop* shop;
};

