#ifndef _FLOWER_PROJECTILE_INCLUDE
#define _FLOWER_PROJECTILE_INCLUDE

#include "NonPlayerEntity.h"

class FlowerProjectile : public NonPlayerEntity {

public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);

	virtual void onEntityHit(bool isPlayer = true);

	void setDirection(bool right) { goingRight = right; }

	void setStartY(float y) { startY = y; }

private:
	bool goingRight = false;
	float attackAngle = 0;
	float startY = 0;
};

#endif // _FLOWER_PROJECTILE_INCLUDE
