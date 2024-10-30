#include "BreakeableBlock.h"
#include "Game.h"
#include "irrKlang.h"


#define OFFSET_X 0.125
#define OFFSET_Y 0.5

void BreakeableBlock::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, irrklang::ISoundEngine* soundEngine,irrklang::ISoundSource* boxBreaking) {
	dead = false;
	spritesheet.loadFromFile("images/blocks.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(OFFSET_X, OFFSET_Y), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(0, 1);
	sprite->addKeyframe(0, glm::vec2(2 * OFFSET_X, 0.f));

	sprite->setAnimationSpeed(1, 1);
	sprite->addKeyframe(1, glm::vec2(2 * OFFSET_X, 0.f));

	sprite->changeAnimation(1);
	tileMapDispl = glm::vec2(tileMapPos);
	hitbox_x = 16;
	hitbox_y = 16;
	tileSize = map->getTileSize();
	tileX = pos.x / tileSize;
	tileY = pos.y / tileSize;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	this->soundEngine = soundEngine;
	this->boxBreaking = boxBreaking;
}

void BreakeableBlock::onEntityHit(bool isPlayer) {
	player->addScore(10);
	if (boxBreaking && !soundEngine->isCurrentlyPlaying(boxBreaking->getName())) {
		soundEngine->play2D(boxBreaking);
	}
	dead = true;
}

