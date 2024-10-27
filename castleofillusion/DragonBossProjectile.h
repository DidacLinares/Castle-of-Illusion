#ifndef _DRAGON_BOSS_PROJECTILE_INCLUDE
#define _DRAGON_BOSS_PROJECTILE_INCLUDE


#include "NonPlayerEntity.h"
class DragonBossProjectile : public NonPlayerEntity {
public:
	enum DIRECTIONS {
		LEFT, DOWN, RIGHT
	};

	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);

	virtual void onEntityHit(bool isPlayer = true);

	void setDirection(int direction) { this->direction = direction; }
private:
	int direction = 0;
};

#endif // _DRAGON_BOSS_PROJECTILE_INCLUDE

