#ifndef _BREAKEABLE_BLOCK_INCLUDE
#define _BREAKEABLE_BLOCK_INCLUDE
#include "Block.h"

class BreakeableBlock : public Block {
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void onEntityHit(bool isPlayer = true);
};
#endif // _BREAKEABLE_BLOCK_INCLUDE

