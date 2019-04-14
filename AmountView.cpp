#include "AmountView.h"

using namespace std;

AmountView::AmountView(GLuint* f, glm::vec3 p, GLint s, int startChar, glm::vec4 color)
	: UIElement(f, p, s), colorMod(color) {
	amount = 0;

	view.push_back(new Renderable(position, font[startChar], numElements));
	view.push_back(new Renderable(position + glm::vec3(0.15f, 0.f, 0.f), font[0], numElements));
}

void AmountView::add(int addition) {
	int oldLength = to_string(amount).length();
	amount += addition;
	int newLength = to_string(amount).length();

	// old < new
	for (int i = oldLength + 1; i < newLength + 1; ++i) {
		view.push_back(new Renderable(position + glm::vec3(i * 0.15f, 0.f, 0.f), font[0], numElements));
	}

	// new < old
	for (int i = newLength; i < oldLength; ++i) {
		view.pop_back();
	}

	for (int i = 1; i < view.size(); ++i) {
		view[i]->setTex(font[(amount / (int)pow(10, view.size() - 1 - i) % 10)]);
	}
}

void AmountView::render(Shader &shader) {
	shader.setUniform4f("colorMod", colorMod);
	for (Renderable* i : view) {
		i->render(shader);
	}
}
