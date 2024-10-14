#ifndef _CAKE_INCLUDE
#define _CAKE_INCLUDE

#include "Entity.h"
#include "Player.h"

class Cake : public Entity {
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	virtual void onEntityHit();
	
	virtual glm::vec4 getCollisionBox();
	void setPlayer(Player* player);


	virtual bool checkCollision();
private:
	Player* player;
};


#endif // _CAKE_INCLUDE