#ifndef _BLOCK_INCLUDE
#define _BLOCK_INCLUDE


#include "NonPlayerEntity.h"

class Block : public NonPlayerEntity
{
public:

	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	virtual void onEntityHit(bool IsPlayer = true);

	bool checkCollision(glm::vec4 hitboxentity);
protected:
	bool pickedUp = false, transition = false, falling = false,throwed = false,left = false;
	int tileX, tileY,tileSize;
	bool goingUp = false;
	int count = 0;
	vector<NonPlayerEntity*> entities;

};

#endif // _BLOCK_INCLUDE
