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
	void setSoundEngineAndSounds(irrklang::ISoundEngine* soundEngine, irrklang::ISoundSource* jumpSound, irrklang::ISoundSource* hitSound);
	void grabAnimation();
	bool getObject();
	bool moving();
	void setObject(bool object);

	int getLives();
	int getScore();
	void addScore(int points);
	void changeAnim(int anim);
	void setDying(bool die) { dying = die; }

private:

	irrklang::ISoundEngine* soundEngine;
	irrklang::ISoundSource* jumpSound; 
	irrklang::ISoundSource* hitSound;
	bool bJumping,falling, crouching, groundpounding, hit;
	int jumpAngle;
	float speedX,speedY, startY,maxY,startX = 0;
	int score = 0;
	bool godMode = false, object = false, dying = false, shortenedJump = false, deadMovement = false;
	int deathTime = 0;
	int deathAngle = 0;
	bool musicDeathfase = false;
	bool firstDyingIteration = true;
};


#endif // _PLAYER_INCLUDE


