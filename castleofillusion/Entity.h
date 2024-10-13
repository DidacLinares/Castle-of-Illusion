#ifndef _ENTITY_INCLUDE
#define _ENTITY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Entity {

public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) = 0;
	virtual void update(int deltaTime) = 0;
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition();
	virtual glm::vec4 getCollisionBox() = 0;

	void setLives(int lives);
	int getLives();

	bool isInvulnerable();
	void setInvulnerable(bool invulnerable);

	bool isDead();
	void setDead(bool dead);

	virtual void onEntityHit() = 0;

protected:
	glm::vec2 tileMapDispl, pos;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	bool dead = false;
	bool invulnerable;

	float invulnerableTimeLeft;
	float invulnerableDuration = 1500.0f; // 1.5 seconds in milliseconds

	int lives = 3;
};


#endif // _ENTITY_INCLUDE


#pragma once
