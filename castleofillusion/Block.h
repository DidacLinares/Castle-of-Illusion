#ifndef _BLOCK_INCLUDE
#define _BLOCK_INCLUDE


#include "NonPlayerEntity.h"


class Block : public NonPlayerEntity
{
public:

	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	virtual void onEntityHit();	
private:
	bool pickedUp = false;
	int tileX, tileY;

};

#endif // _BLOCK_INCLUDE
