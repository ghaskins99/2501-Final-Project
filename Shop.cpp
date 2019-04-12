#include "Shop.h"

using namespace std;

Shop::Shop(GLuint* f, glm::vec3 p, GLint s, vector<GLuint> t)
	: UIElement(f, p, s) {
	for (int i = 0; i < t.size(); ++i) {
		glm::vec3 pos = position + glm::vec3(1.0f, (i+1) * -3.f, 0.0f);
		costs.push_back(new AmountView(f, pos, s, 10, glm::vec3(0.9f, 0.9f, 0.0f)));
		costs[i]->add(50);

		items.push_back(new Renderable(pos + glm::vec3(-0.5f, 1.f, 0.0f), t[i], s));
		items[i]->setScale(glm::vec3(6.f, 6.f, 0.0f));
	}
}

void Shop::render(Shader &shader) {
	for (AmountView* a : costs) {
		a->render(shader);
	}

	shader.setUniform3f("colorMod", glm::vec3(0.0f));

	for (Renderable* i : items) {
		i->render(shader);
	}
}