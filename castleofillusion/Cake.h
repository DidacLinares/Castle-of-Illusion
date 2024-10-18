#ifndef _CAKE_INCLUDE
#define _CAKE_INCLUDE

#include "NonPlayerEntity.h"

class Cake : public NonPlayerEntity {
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	virtual void onEntityHit();
	
	virtual glm::vec4 getCollisionBox();
};


#endif // _CAKE_INCLUDE