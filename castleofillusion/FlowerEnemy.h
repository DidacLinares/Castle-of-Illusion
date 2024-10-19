#ifndef _FLOWERENEMY_INCLUDE
#define _FLOWERENEMY_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Entity.h"

#pragma once
class FlowerEnemy : public Entity {

public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	virtual glm::vec4 getCollisionBox();

	void onEntityHit();

private:
	bool dying = false;
	int deathFase = 1;
	int deathTime = 0;
};

#endif // _FLOWERENEMY_INCLUDE