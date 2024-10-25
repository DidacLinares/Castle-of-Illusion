#ifndef _COIN_INCLUDE
#define _COIN_INCLUDE

#include "NonPlayerEntity.h"

class Coin : public NonPlayerEntity {
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	virtual void onEntityHit(bool IsPlayer = true);

};


#endif // _COIN_INCLUDE