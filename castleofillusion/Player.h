#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Entity.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player : public Entity {

public:
	virtual void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	virtual void update(int deltaTime);
	virtual void render();
	
	virtual glm::vec4 getCollisionBox();

	bool isPlayerGroundPounding();
	bool movingLeft();
	bool isHit();

	void removeLive();
	void checkGroundCollision();
	
private:
	bool bJumping,falling, crouching, groundpounding,hit;
	int jumpAngle;
	float speedX,speedY, startY;
	int max = 800,lives = 3;

};


#endif // _PLAYER_INCLUDE


