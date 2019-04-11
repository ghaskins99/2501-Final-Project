#include <algorithm>
#include "UIController.h"
using namespace std;

UIController::UIController(GLuint* fontTextures, GLuint s) {
	fakefont = fontTextures;
	size = s;

	backdrop = new Renderable(glm::vec3(4.0f, 4.0f, 2.0f), fakefont[12], size);
	backdrop->setScale(glm::vec3(10.0f, 100.0f, 1.0f));
	
	moneyView.push_back(new Renderable(startPos, fakefont[10], size));
	initValue(moneyView, &money, startPos);
	addValue(moneyView, &money, 100, startPos);

	glm::vec3 offset = startPos + glm::vec3(7.5 * 0.15f, 0.0f, 0.0f);
	healthView.push_back(new Renderable(offset, fakefont[11], size));
	initValue(healthView, &health, offset);
	addValue(healthView, &health, 100, offset);
}

void UIController::initValue(std::vector<Renderable*>& view, int* val, glm::vec3 offset) {
	*val = 0;

	view.push_back(new Renderable(offset + glm::vec3(0.15f, 0.f, 0.f), fakefont[0], size));
}

void UIController::addValue(std::vector<Renderable*>& view, int* val, int addition, glm::vec3 offset) {
	int oldLength = to_string(*val).length();
	int newLength = to_string(*val + addition).length();

	for (int i = oldLength + 1; i < newLength + 1; ++i) {
		view.push_back(new Renderable(offset + glm::vec3(i * 0.15f, 0.f, 0.f), fakefont[0], size));
	}

	*val += addition;

	for (int i = 1; i < view.size(); ++i) {
		view[i]->setTex(fakefont[(*val / (int)pow(10, view.size() - 1 - i) % 10)]);
	}
}

void UIController::render(Shader &shader) {
	shader.setUniform3f("colorMod", glm::vec3(0.0f, 0.0f, 0.2f));
	backdrop->render(shader);

	shader.setUniform3f("colorMod", glm::vec3(0.9f, 0.9f, .0f));
	for (Renderable* m : moneyView) {
		m->render(shader);
	}

	shader.setUniform3f("colorMod", glm::vec3(0.9f, 0.0f, 0.0f));
	for (Renderable* h : healthView) {
		h->render(shader);
	}

	shader.setUniform3f("colorMod", glm::vec3(0.0f));
}