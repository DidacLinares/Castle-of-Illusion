#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include <irrKlang.h>
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
	
	bool isPlayerGroundPounding();
	bool movingLeft();
	bool isHit();

	virtual void onEntityHit(bool isPlayer = true);
	void checkGroundCollision();

	void setGodMode(bool godMode);
	bool isGodMode();
	bool checkCollision(glm::vec4 hitboxentity);
	void setSoundEngine(irrklang::ISoundEngine* soundEngine);
	void grabAnimation();
	bool getObject();
	bool moving();
	void setObject(bool object);

	int getLives();
	int getTries();
	int getScore();

private:
	irrklang::ISoundEngine* soundEngine;
	irrklang::ISoundSource* jumpSound; 
	bool bJumping,falling, crouching, groundpounding, hit;
	int jumpAngle;
	float speedX,speedY, startY;
	int tries = 3,score = 123456;
	bool godMode = false,object = false;
};


#endif // _PLAYER_INCLUDE


