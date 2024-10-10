#ifndef _TREEENEMY_INCLUDE
#define _TREEENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class TreeEnemy {

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::vec2 getPosition();
	void setPlayer(Player* player);
	bool movingLeft();
	
private:
	glm::vec2 tileMapDispl, posEnemy;
	int jumpAngle;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	Player* player;
	bool checkCollision();
};


#endif // _TREEENEMY_INCLUDE


