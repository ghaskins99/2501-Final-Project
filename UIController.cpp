#include <algorithm>
#include "UIController.h"
using namespace std;

UIController::UIController(GLuint* fontTextures, GLint s, std::vector<GLuint> t) {
	fakefont = fontTextures;
	numElements = s;

	backdrop = new Renderable(glm::vec3(4.0f, 4.0f, 2.0f), fakefont[12], numElements);
	backdrop->setScale(glm::vec3(10.0f, 100.0f, 1.0f));

	cancel = new Renderable(startPos + glm::vec3(1.5f, -1.f, 0.f), fakefont[13], numElements);
	
	money = new AmountView(fakefont, startPos, numElements, 10, glm::vec4(0.9f, 0.9f, 0.0f, 0.0f));
	money->add(100);

	glm::vec3 offset = startPos + glm::vec3(7.5 * 0.15f, 0.0f, 0.0f);
	health = new AmountView(fakefont, offset, numElements, 11, glm::vec4(0.9f, 0.0f, 0.0f, 0.0f));
	health->add(100);

	shop = new Shop(fakefont, startPos, numElements, t);
}

void UIController::render(Shader &shader) {
	shader.setUniform4f("colorMod", glm::vec4(0.0f, 0.0f, 0.2f, 0.0f));
	backdrop->render(shader);

	money->render(shader);

	health->render(shader);

	cancel->render(shader);

	shop->render(shader);

	shader.setUniform4f("colorMod", glm::vec4(0.0f));
}

void UIController::changeMoney(int amt) {
	money->add(amt);
}

void UIController::changeHealth(int amt) {
	health->add(amt);
}

int UIController::getShopCost(int num) {
	return shop->getCosts()[num]->getAmount();
}