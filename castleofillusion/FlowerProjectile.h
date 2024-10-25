#pragma once
#include "NonPlayerEntity.h"

class FlowerProjectile : public NonPlayerEntity {

public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);

	virtual void onEntityHit(bool isPlayer = true);

	void setDirection(bool right) { goingRight = right; }

private:
	bool goingRight = false;
};

