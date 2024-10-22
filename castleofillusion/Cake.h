#ifndef _CAKE_INCLUDE
#define _CAKE_INCLUDE

#include "NonPlayerEntity.h"

class Cake : public NonPlayerEntity {
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	virtual void onEntityHit(bool IsPlayer = true);
	
};


#endif // _CAKE_INCLUDE