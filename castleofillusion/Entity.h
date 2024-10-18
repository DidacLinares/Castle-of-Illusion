#ifndef _ENTITY_INCLUDE
#define _ENTITY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

class Entity {

public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) = 0;
	virtual void update(int deltaTime) = 0;
	virtual void render();

	virtual glm::vec4 getCollisionBox() = 0;
	virtual void onEntityHit() = 0;

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
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
	glm::vec4 hitbox;

	float invulnerableTimeLeft;
	float invulnerableDuration = 1500.0f; // 1.5 seconds in milliseconds

	int lives;
};


#endif // _ENTITY_INCLUDE