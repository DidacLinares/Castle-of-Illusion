#ifndef _ENTITY_INCLUDE
#define _ENTITY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

class Entity {

public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) = 0;
	virtual void update(int deltaTime) = 0;
	virtual void render();

	glm::vec4 getCollisionBox();
	virtual void onEntityHit(bool isPlayer = true) = 0;

	void setTileMap(TileMap* tileMap);
	virtual void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition();

	void setLives(int lives);
	int getLives();

	bool isInvulnerable();
	void setInvulnerable(bool invulnerable);

	bool isDead();
	void setDead(bool dead);

protected:
	glm::vec2 tileMapDispl, pos;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	bool dead = false;
	bool invulnerable;
	float hitbox_x, hitbox_y;

	float invulnerableTimeLeft;
	float invulnerableDuration = 1500.0f; // 1.5 seconds in milliseconds

	int lives;
};


#endif // _ENTITY_INCLUDE