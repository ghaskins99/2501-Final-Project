#pragma once
#include "TowerObject.h"
#include "EnemyObject.h"
#include <vector>

class FlameTower :
	public TowerObject
{
public:
	FlameTower(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, GLuint bulletTex);
	~FlameTower();

	virtual void update(float deltaTime, std::vector<EnemyObject *> targets);
	void render(Shader &shader);
	void updateBullets(float deltaTime, std::vector<EnemyObject *> targets);

	inline virtual std::string getType() { return "flame"; }
	inline bool isFiring() { return isShooting; }
private:
	EnemyObject* target;
	float sep;
	bool isShooting;
};

