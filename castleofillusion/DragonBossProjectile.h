#ifndef _DRAGON_BOSS_PROJECTILE_INCLUDE
#define _DRAGON_BOSS_PROJECTILE_INCLUDE


#include "NonPlayerEntity.h"
class DragonBossProjectile : public NonPlayerEntity {
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);

	virtual void onEntityHit(bool isPlayer = true);
private:
};

#endif // _DRAGON_BOSS_PROJECTILE_INCLUDE

