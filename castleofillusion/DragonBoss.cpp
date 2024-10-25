#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "DragonBoss.h"
#include "Game.h"

#define OFFSET_BODY_X 0.33
#define OFFSET_HEAD_X 0.33

enum DragonBossBodyAnim {
	DUMMY, BODY
};

enum DragonBossHeadAnim {
	HEAD_DOWN_LEFT, HEAD_DOWN_RIGHT, HEAD_CLOSE_LEFT, HEAD_CLOSE_RIGHT, HEAD_LEFT, HEAD_RIGHT
};

void DragonBoss::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	dead = false;
	lives = 3;
	spritesheet.loadFromFile("images/DragonBossBody.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(75, 110), glm::vec2(OFFSET_BODY_X, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(DUMMY, 1);
	sprite->addKeyframe(DUMMY, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(BODY, 2);
	sprite->addKeyframe(BODY, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(BODY, glm::vec2(OFFSET_BODY_X, 0.f));
	sprite->addKeyframe(BODY, glm::vec2(2 * OFFSET_BODY_X, 0.f));
	sprite->addKeyframe(BODY, glm::vec2(OFFSET_BODY_X, 0.f));
	sprite->addKeyframe(BODY, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(BODY);

	headSpriteSheet.loadFromFile("images/DragonBossHead.png", TEXTURE_PIXEL_FORMAT_RGBA);
	headSprite = Sprite::createSprite(glm::ivec2(50, 32), glm::vec2(OFFSET_HEAD_X, 1), &headSpriteSheet, &shaderProgram);

	headSprite->setNumberAnimations(6);

	headSprite->setAnimationSpeed(HEAD_DOWN_LEFT, 1);
	headSprite->addKeyframe(HEAD_DOWN_LEFT, glm::vec2(0.f, 0.f));

	headSprite->setAnimationSpeed(HEAD_DOWN_RIGHT, 1);
	headSprite->addKeyframe(HEAD_DOWN_RIGHT, glm::vec2(0.f, 0.f));

	headSprite->setAnimationSpeed(HEAD_CLOSE_LEFT, 1);
	headSprite->addKeyframe(HEAD_CLOSE_LEFT, glm::vec2(OFFSET_HEAD_X, 0.f));

	headSprite->setAnimationSpeed(HEAD_CLOSE_RIGHT, 1);
	headSprite->addKeyframe(HEAD_CLOSE_RIGHT, glm::vec2(OFFSET_HEAD_X, 0.f));

	headSprite->setAnimationSpeed(HEAD_LEFT, 1);
	headSprite->addKeyframe(HEAD_LEFT, glm::vec2(2 * OFFSET_HEAD_X, 0.f));

	headSprite->setAnimationSpeed(HEAD_RIGHT, 1);
	headSprite->addKeyframe(HEAD_RIGHT, glm::vec2(2 * OFFSET_HEAD_X, 0.f));

	headSprite->changeAnimation(HEAD_DOWN_LEFT);

	pos.x = 0;
	pos.y = 0;
	tileMapDispl = glm::vec2(tileMapPos);
	hitbox_x = 75;
	hitbox_y = 110;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	headSprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void DragonBoss::update(int deltaTime) {
	headSprite->update(deltaTime);
	sprite->update(deltaTime);

	if (dead) {
		return;
	}

	invulnerableTimeLeft -= deltaTime;

	if (invulnerableTimeLeft <= 0) {
		invulnerable = false;
	}

	float distanceToPlayer = glm::length(player->getPosition() - pos);


}

void DragonBoss::render() {
	if (dead) {
		return;
	}

	sprite->render();
	headSprite->render();
}


void DragonBoss::onEntityHit(bool isPlayer) {
	if (!isPlayer) {
		if (this->isInvulnerable()) return;
		
		--lives;
		this->setInvulnerable(true);

		if (lives == 0) {
			dead = true;
		}

		return;
	}

	if (player->isInvulnerable()) {
		return;
	}

	player->onEntityHit();
}

void DragonBoss::setPosition(const glm::vec2& pos) {
	this->pos = pos;
	sprite->setPosition(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));

	if (headSprite->animation() == HEAD_DOWN_LEFT || headSprite->animation() == HEAD_DOWN_RIGHT) {
		headSprite->setPosition(glm::vec2(tileMapDispl.x + pos.x + 18, tileMapDispl.y + pos.y + 11));
	}
	else {
		headSprite->setPosition(glm::vec2(tileMapDispl.x + pos.x + 25, tileMapDispl.y + pos.y + 11));
	}
}