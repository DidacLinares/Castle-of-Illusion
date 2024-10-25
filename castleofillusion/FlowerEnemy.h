#ifndef _FLOWERENEMY_INCLUDE
#define _FLOWERENEMY_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "NonPlayerEntity.h"

#pragma once
class FlowerEnemy : public NonPlayerEntity {

public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);

	virtual void onEntityHit(bool isPlayer = true);

private:
	bool dying = false;
	int deathFase = 1;
	int deathTime = 0;

	float startX, startY = 0;
	int deathAngle = 0;

	int timeSinceLastAttack = 0;
	bool attacking = false;
};

#endif // _FLOWERENEMY_INCLUDE