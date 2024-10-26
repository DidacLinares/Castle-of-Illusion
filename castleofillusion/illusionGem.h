#ifndef _ILLUSIONGEM_INCLUDE
#define _ILLUSIONGEM_INCLUDE

#include "NonPlayerEntity.h"

class IllusionGem : public NonPlayerEntity {
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	virtual void onEntityHit(bool isPlayer = true);

};


#endif // _ILLUSIONGEM_INCLUDE

