#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "DragonBoss.h"
#include "Game.h"
#include "DragonBossProjectile.h"

#define OFFSET_BODY_X 0.33
#define OFFSET_HEAD_X 0.33

#define ATTACK_COOLDOWN 1400
#define CHANGE_HEAD_POSITION 167
#define ATTACKING_TIME 258

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

	headSprite->changeAnimation(animCycle);

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

	if (player->checkCollision(getCollisionBox())) {
		onEntityHit();
	}

	invulnerableTimeLeft -= deltaTime;

	if (invulnerableTimeLeft <= 0) {
		invulnerable = false;
	}
		
	if (timeSinceLastAttack <= ATTACK_COOLDOWN) {
		timeSinceLastAttack += deltaTime;
	}
	else {
		// ATTACK
		timeSinceLastAttack = 0;
		attack = true;
	}

	if (attacking) {
		attackingTime += deltaTime;
		if (attackingTime >= ATTACKING_TIME) {
			attackingTime = 0;
			attacking = false;
		}
	}

	if (!attacking) {
		timeSinceLastHeadChange += deltaTime;
		if (timeSinceLastHeadChange >= CHANGE_HEAD_POSITION) {
			timeSinceLastHeadChange = 0;
			headSprite->changeAnimation(animCycle);
			adjustHeadPosition(animCycle);
			animCycle = (animCycle + 1) % 6;
		}
	}

	if (attack) {
		attack = false;
		for (int i = 0; i < 3; ++i) {
			spawnProjectile(i);
		}
		attacking = true;
	}
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

void DragonBoss::adjustHeadPosition(int animation) {
	if (animation == HEAD_DOWN_LEFT || animation == HEAD_DOWN_RIGHT) {
		headSprite->setPosition(glm::vec2(tileMapDispl.x + pos.x + 18, tileMapDispl.y + pos.y + 11));
		return;
	}

	switch (animation) {
		case HEAD_CLOSE_LEFT:
			headSprite->setPosition(glm::vec2(tileMapDispl.x + pos.x + 25, tileMapDispl.y + pos.y + 11));
			break;
		case HEAD_CLOSE_RIGHT:
			headSprite->setPosition(glm::vec2(tileMapDispl.x + pos.x + 13, tileMapDispl.y + pos.y + 11));
			break;
		case HEAD_LEFT:
			headSprite->setPosition(glm::vec2(tileMapDispl.x + pos.x + 25, tileMapDispl.y + pos.y + 11));
			break;
		case HEAD_RIGHT:
			headSprite->setPosition(glm::vec2(tileMapDispl.x + pos.x + 13, tileMapDispl.y + pos.y + 11));
			break;
		default:
			break;
	}
}

void DragonBoss::spawnProjectile(int i) {
	float projX, projY = 0.0;

	switch (headSprite->animation()) {
		case HEAD_DOWN_LEFT:
		case HEAD_DOWN_RIGHT:
			projX = pos.x + 13;
			projY = pos.y + 27;
			break;

		case HEAD_CLOSE_LEFT:
			projX = pos.x + 2;
			projY = pos.y + 28;
			break;

		case HEAD_CLOSE_RIGHT:
			projX = pos.x + 30;
			projY = pos.y + 29;
			break;

		case HEAD_LEFT:
			projX = pos.x - 10;
			projY = pos.y + 22;
			break;

		case HEAD_RIGHT:
			projX = pos.x + 40;
			projY = pos.y + 22;
			break;
	}



	int direction = i % 3;
	DragonBossProjectile* projectile = new DragonBossProjectile();
	projectile->init(tileMapDispl, Game::instance().getScene()->getShaderProgram());
	projectile->setTileMap(map);
	projectile->setPosition(glm::vec2(projX, projY));
	projectile->setDirection(direction);
	projectile->setPlayer(player);
	Game::instance().getScene()->addEntity(projectile);
}
