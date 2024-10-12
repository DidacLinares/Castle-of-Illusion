#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "TreeEnemy.h"
#include "Game.h"

#define OFFSET_X 0.25
#define OFFSET_Y 0

#define HITBOX_X 24
#define HITBOX_Y 32

enum TreeAnims {
	MOVE_LEFT, MOVE_RIGHT, DIE_LEFT, DIE_RIGHT 
};


void TreeEnemy::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	dead = false;
	spritesheet.loadFromFile("images/TreeEnemy.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 32), glm::vec2(OFFSET_X, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE_LEFT, 3);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(OFFSET_X, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(2 * OFFSET_X, 0.0f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 3);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(OFFSET_X, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * OFFSET_X, 0.0f));


	sprite->setAnimationSpeed(DIE_LEFT, 1);
	sprite->addKeyframe(DIE_LEFT, glm::vec2(3 * OFFSET_X, 0.f));

	sprite->setAnimationSpeed(DIE_RIGHT, 1);
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(3 * OFFSET_X, 0.f));

	sprite->changeAnimation(1);
	pos.x = 0;
	pos.y = 0;
	tileMapDispl = glm::vec2(tileMapPos);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	
}

void TreeEnemy::update(int deltaTime) {
	sprite->update(deltaTime);
	if (sprite->animation() == MOVE_LEFT) pos.x -= 2;
	else pos.x += 2;

	if (map->collisionMoveRight(pos, glm::vec2(HITBOX_X, HITBOX_Y))) {
		pos.x -= 2;
		sprite->changeAnimation(MOVE_LEFT);
	}
	if (map->collisionMoveLeft(pos, glm::vec2(HITBOX_X, HITBOX_Y))) {
		pos.x += 2;
		sprite->changeAnimation(MOVE_RIGHT);
	}
	if (checkCollision()) {
		onEntityHit();
	}
	sprite->setPosition(glm::vec2(int(tileMapDispl.x + pos.x), int(tileMapDispl.y + pos.y)));
}



glm::vec4 TreeEnemy::getCollisionBox() {
	return glm::vec4(pos.x, pos.y, HITBOX_X, HITBOX_Y);
}

bool TreeEnemy::checkCollision() {
	glm::vec4 hitboxplayer = player->getCollisionBox();
	glm::vec4 hitboxenemy = glm::vec4(pos.x, pos.y, HITBOX_X, HITBOX_Y);
	return (hitboxplayer.x < hitboxenemy.x + hitboxenemy.z && // hitboxplayer.left < hitboxenemy.right
		hitboxplayer.x + hitboxplayer.z > hitboxenemy.x && // hitboxplayer.right > hitboxenemy.left
		hitboxplayer.y < hitboxenemy.y + hitboxenemy.w && // hitboxplayer.top < hitboxenemy.bottom
		hitboxplayer.y + hitboxplayer.w > hitboxenemy.y);  // hitboxplayer.bottom > hitboxenemy.top
}

void TreeEnemy::setPlayer(Player *player) {
	this->player = player;
}

void TreeEnemy::onEntityHit() {
	if (player->isPlayerGroundPounding()) {
		dead = true;
	}
	else {
		if (player->isInvulnerable()) {
			cout << "Mickey is invulnerable!" << endl;
		}
		else {
			player->onEntityHit();
		}
	}
}
