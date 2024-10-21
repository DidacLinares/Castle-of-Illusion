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
	hitbox_y = 16;
	tileSize = map->getTileSize();
	tileX = pos.x / tileSize;
	tileY = pos.y / tileSize;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Block::update(int deltaTime) {
	pos.y += 4;
	map->collisionMoveDown(pos, glm::vec2(hitbox_x, hitbox_y), &pos.y);
	if (Game::instance().getKey(GLFW_KEY_F) && !transition) {
		transition = true;
		Game::instance().keyReleased(GLFW_KEY_F);
	}
	if (transition && !pickedUp) {
		transition = false;
		if (map->setTileAsBlock(tileX, tileY, 0)) {
			pickedUp = true;
		};
	}
	if (pickedUp && !falling) {
		pos = player->getPosition();
	}
	if (falling) {
		pos.y += 4;
		if (map->collisionMoveDown(pos, glm::vec2(hitbox_x, hitbox_y), &pos.y)) {
			tileY = pos.y / tileSize;
			if (map->setTileAsBlock(tileX, tileY, 1000)) {
				pickedUp = false;
				falling = false;
			}
		}
	}
	if (transition && pickedUp) {
		transition = false;
		pos = player->getPosition();
		int tileXtmp = pos.x / tileSize;
		int tileYtmp = pos.y / tileSize;
		tileX = tileXtmp + 1; // Cambiar a colocar cap a on mira (if movingLeft -> -1 i no + 1)
		tileY = tileYtmp + 1;
		pos.x = tileX * tileSize;
		pos.y = tileY * tileSize;
		pos.y += 4;
		bool fallingtmp = !map->collisionMoveDown(pos, glm::vec2(hitbox_x, hitbox_y), &pos.y);
		if (fallingtmp && map->isTileValidAsBlock(tileX, tileY)) {
			falling = true;
		}
		else if (!fallingtmp) {
			if (map->setTileAsBlock(tileX, tileY, 1000)) {
				pickedUp = false;
				player->setPosition(glm::vec2(tileXtmp * tileSize, tileYtmp * tileSize));
			}
		}
	} 
	if (!pickedUp) {
		tileX = pos.x / tileSize;
		tileY = pos.y / tileSize;
		map->setTileAsBlock(tileX, tileY,1000);
    }
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Block::onEntityHit() {
	
}