#pragma once
#include "TowerObject.h"
#include "EnemyObject.h"

class SlowTower :
	public TowerObject
{
public:
	SlowTower(glm::vec3 &entityPos, GLuint entityTexture, GLint entityNumElements, GLuint bulletTex);
	~SlowTower();

	virtual void update(float deltaTime, std::vector<EnemyObject *> targets);
	void render(Shader &shader);

	inline virtual std::string getType() { return "slow"; }

private:
	std::vector<EnemyObject*> targetsInRange;
	float range;
};

