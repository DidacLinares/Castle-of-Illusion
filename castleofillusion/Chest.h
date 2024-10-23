#ifndef _CHEST_INCLUDE
#define _CHEST_INCLUDE
#include "Block.h"

class Chest : public Block {
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void onEntityHit(bool isPlayer = true);
	void setObjectToSpawn(int id);
private:
	int objectToSpawn;
};
#endif // _CHEST_INCLUDE 
