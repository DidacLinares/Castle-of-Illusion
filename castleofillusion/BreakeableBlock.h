#ifndef _BREAKEABLE_BLOCK_INCLUDE
#define _BREAKEABLE_BLOCK_INCLUDE
#include "Block.h"
#include "irrKlang.h"

class BreakeableBlock : public Block {
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, irrklang::ISoundEngine* soundEngine, irrklang::ISoundSource* boxBreaking);
	virtual void onEntityHit(bool isPlayer = true);
private:
	irrklang::ISoundEngine* soundEngine;
	irrklang::ISoundSource* boxBreaking;
};
#endif // _BREAKEABLE_BLOCK_INCLUDE

