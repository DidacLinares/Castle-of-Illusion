#ifndef _DRAGON_BOSS_PROJECTILE_INCLUDE
#define _DRAGON_BOSS_PROJECTILE_INCLUDE


#include "NonPlayerEntity.h"
class DragonBossProjectile : public NonPlayerEntity {
public:
	enum DIRECTIONS {
		LEFT, DOWN, RIGHT
	};

	enum HEAD_POSITIONS {
		HEAD_LEFT, HEAD_DOWN, HEAD_RIGHT
	};

	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);

	virtual void onEntityHit(bool isPlayer = true);

	void setDirection(int direction) { this->direction = direction; }

	void setHeadPosition(int position) { headPosition = position; }
private:
	int direction = 0;
	int headPosition = 0;
};

#endif // _DRAGON_BOSS_PROJECTILE_INCLUDE

