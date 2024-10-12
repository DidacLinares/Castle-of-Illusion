#ifndef _TREEENEMY_INCLUDE
#define _TREEENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Entity.h"
#include "Player.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class TreeEnemy: public Entity {

public:
	virtual void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	virtual void update(int deltaTime);
	virtual glm::vec4 getCollisionBox();


	void setPlayer(Player* player);	
private:
	Player* player;
	bool checkCollision();
};


#endif // _TREEENEMY_INCLUDE


