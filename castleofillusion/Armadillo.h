#ifndef _ARMADILLO_INCLUDE
#define _ARMADILLO_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "NonPlayerEntity.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Armadillo : public NonPlayerEntity {

public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);

	virtual void onEntityHit(bool player = true);

private:
	bool dying = false, rolling = false;;
	int deathFase = 1;
	int deathTime = 0;
	int deathAngle = 0;
	int rollTimer = 0;
	int unrollTimer = 0;

	float startX, startY = 0;

};


#endif // _ARMADILLO_INCLUDE


