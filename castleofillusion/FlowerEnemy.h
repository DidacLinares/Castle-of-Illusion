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

	void onEntityHit();

private:
	bool dying = false;
	int deathFase = 1;
	int deathTime = 0;
};

#endif // _FLOWERENEMY_INCLUDE