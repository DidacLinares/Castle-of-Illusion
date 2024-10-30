#ifndef _CHEST_INCLUDE
#define _CHEST_INCLUDE
#include "Block.h"
#include "irrKlang.h"

class Chest : public Block {
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, irrklang::ISoundEngine* soundEngine, irrklang::ISoundSource* boxBreaking);
	virtual void update(int deltaTime);
	virtual void onEntityHit(bool isPlayer = true);
	void setObjectToSpawn(int id);
private:
	int objectToSpawn;
	irrklang::ISoundEngine* soundEngine;
	irrklang::ISoundSource* boxBreaking;
};
#endif // _CHEST_INCLUDE 
