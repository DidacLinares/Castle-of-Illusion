#pragma once
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

	void onEntityHit();

private:
	int lives = 3;
	bool dying = false;
	int deathFase = 1;
	int deathAngle = 0;

	Texture headSpriteSheet;
	Sprite* headSprite;

	float startX, startY = 0;
};

#endif // _DRAGON_BOSS_INCLUDE