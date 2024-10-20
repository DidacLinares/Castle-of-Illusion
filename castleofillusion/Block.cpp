#include "Block.h"
#include <iostream>
#include "Game.h"

#define OFFSET_X 0.125
#define OFFSET_Y 0.5

#define HITBOX_X 16
#define HITBOX_Y 16


void Block::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
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
	hitbox_y = 32;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Block::update(int deltaTime) {
	pos.y += 4;
	map->collisionMoveDown(pos, glm::vec2(hitbox_x, hitbox_y), &pos.y);
	if (Game::instance().getKey(GLFW_KEY_F)) {
		pickedUp = !pickedUp;
		Game::instance().keyReleased(GLFW_KEY_F);
	}
		
	if (!pickedUp) {
		tileX = pos.x / map->getTileSize();
		tileY = pos.y / map->getTileSize();
		map->setTileAsBlock(tileX, tileY,1000);
    }
	else {
		map->setTileAsBlock(tileX, tileY,0);
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Block::onEntityHit() {
	
}