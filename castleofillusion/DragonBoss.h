#ifndef _DRAGON_BOSS_INCLUDE
#define _DRAGON_BOSS_INCLUDE

#include "NonPlayerEntity.h"
#include "Sprite.h"
#include "TileMap.h"

class DragonBoss :
	public NonPlayerEntity
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2& pos);

	void onEntityHit(bool isPlayer = true);

private:
	void adjustHeadPosition(int animation);

	void spawnProjectile(int i);

private:
	bool dying = false;
	int deathFase = 1;
	int deathAngle = 0;

	Texture headSpriteSheet;
	Sprite* headSprite;

	int timeSinceLastAttack = 0;
	int timeSinceLastHeadChange = 0;

	int attackingTime = 0;

	int animCycle = 0;

	bool attack = false;
	bool attacking = false;

	float startX, startY = 0;
};

#endif // _DRAGON_BOSS_INCLUDE