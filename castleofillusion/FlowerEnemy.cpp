#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "FlowerEnemy.h"
#include "Game.h"

#define OFFSET_X 0.20
#define OFFSET_Y 0

#define HITBOX_X 24
#define HITBOX_Y 32
#define MAX_RISE_TIME 200
#define MAX_DEATH_TIME 1000


enum FlowerAnims {
	STAND, ATTACK, DIE_LEFT, DIE_RIGHT
};


void FlowerEnemy::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	dead = false;
	spritesheet.loadFromFile("images/FlowerEnemy.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 24), glm::vec2(OFFSET_X, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(STAND, 1);
	sprite->addKeyframe(STAND, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(ATTACK, 2);
	sprite->addKeyframe(ATTACK, glm::vec2(OFFSET_X, 0.f));
	sprite->addKeyframe(ATTACK, glm::vec2(2 * OFFSET_X, 0.0f));

	sprite->setAnimationSpeed(DIE_LEFT, 1);
	sprite->addKeyframe(DIE_LEFT, glm::vec2(3 * OFFSET_X, 0.f));
	
	sprite->setAnimationSpeed(DIE_RIGHT, 1);
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(3 * OFFSET_X, 0.f));

	sprite->changeAnimation(0);
	pos.x = 0;
	pos.y = 0;

	tileMapDispl = glm::vec2(tileMapPos);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));

}

void FlowerEnemy::update(int deltaTime) {
	sprite->update(deltaTime);

	if (!dying) {
		if (!map->collisionMoveDown(pos, glm::vec2(HITBOX_X, HITBOX_Y), &pos.y)) {
			pos.y += 2;
		}
	}

	else {

	}
}

glm::vec4 FlowerEnemy::getCollisionBox() {
	return glm::vec4(pos.x, pos.y, HITBOX_X, HITBOX_Y);
}

void FlowerEnemy::onEntityHit() {

}