#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Armadillo.h"
#include "Game.h"

#define OFFSET_X 0.125
#define OFFSET_Y 0

#define HITBOX_X 24
#define HITBOX_Y 32
#define MAX_RISE_TIME 200
#define MAX_DEATH_TIME 1000
#define DEATH_ANGLE_STEP 4

enum ArmadilloAnims {
	MOVE_LEFT, MOVE_RIGHT, ROLL_LEFT, ROLL_RIGHT, DIE_LEFT, DIE_RIGHT
};


void Armadillo::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	dead = false;
	spritesheet.loadFromFile("images/armadillo.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 32), glm::vec2(OFFSET_X, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);

	sprite->setAnimationSpeed(MOVE_LEFT, 4);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(OFFSET_X, 0.0f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 4);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(OFFSET_X, 0.0f));


	sprite->setAnimationSpeed(ROLL_LEFT, 8);
	sprite->addKeyframe(ROLL_LEFT, glm::vec2(2 * OFFSET_X, 0.0f));
	sprite->addKeyframe(ROLL_LEFT, glm::vec2(3 * OFFSET_X, 0.0f));
	sprite->addKeyframe(ROLL_LEFT, glm::vec2(4 * OFFSET_X, 0.0f));


	sprite->setAnimationSpeed(ROLL_RIGHT, 8);
	sprite->addKeyframe(ROLL_RIGHT, glm::vec2(2 * OFFSET_X, 0.0f));
	sprite->addKeyframe(ROLL_RIGHT, glm::vec2(3 * OFFSET_X, 0.0f));
	sprite->addKeyframe(ROLL_RIGHT, glm::vec2(4 * OFFSET_X, 0.0f));

	sprite->setAnimationSpeed(DIE_LEFT, 1);
	sprite->addKeyframe(DIE_LEFT, glm::vec2(5 * OFFSET_X, 0.f));

	sprite->setAnimationSpeed(DIE_RIGHT, 1);
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(5 * OFFSET_X, 0.f));

	sprite->changeAnimation(1);
	pos.x = 0;
	pos.y = 0;
	tileMapDispl = glm::vec2(tileMapPos);
	hitbox_x = 24;
	hitbox_y = 32;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Armadillo::update(int deltaTime) {
	sprite->update(deltaTime);
	if (!dying) {
		if (rollTimer > 60 * 5) { // cada 5 segons;
			rollTimer = 0;
			rolling = !rolling;
			if (rolling && sprite->animation() == MOVE_LEFT)  sprite->changeAnimation(ROLL_LEFT);
			else if (rolling && sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(ROLL_RIGHT);
			else if (!rolling && sprite->animation() == ROLL_LEFT) sprite->changeAnimation(MOVE_LEFT);
			else if (!rolling && sprite->animation() == ROLL_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
		}
		if (!rolling) {
			if (sprite->animation() == MOVE_LEFT) pos.x -= 2;
			else pos.x += 2;

			if (map->collisionMoveRight(pos, glm::vec2(hitbox_x, hitbox_y))) {
				sprite->changeAnimation(MOVE_LEFT);
			}
			if (map->collisionMoveLeft(pos, glm::vec2(hitbox_x, hitbox_y))) {
				sprite->changeAnimation(MOVE_RIGHT);
			}
			if (player->checkCollision(getCollisionBox())) {
				onEntityHit();
			}
		}
		else {
			if (sprite->animation() == ROLL_LEFT) pos.x -= 4;
			else pos.x += 4;

			if (map->collisionMoveRight(pos, glm::vec2(hitbox_x, hitbox_y))) {
				sprite->changeAnimation(MOVE_LEFT);
				rolling = false;
				rollTimer = 0;
			}
			if (map->collisionMoveLeft(pos, glm::vec2(hitbox_x, hitbox_y))) {
				sprite->changeAnimation(MOVE_RIGHT);
				rolling = false;
				rollTimer = 0;
			}
			if (player->checkCollision(getCollisionBox())) {
				onEntityHit();
			}
		}
		pos.y += 4;
		map->collisionMoveDown(pos, glm::vec2(hitbox_x, hitbox_y), &pos.y);

	}
	else {
		if (sprite->animation() != DIE_RIGHT && sprite->animation() != DIE_LEFT) {
			startX = pos.x;
			startY = pos.y;

			if (sprite->animation() == MOVE_LEFT || sprite->animation() == DIE_LEFT) sprite->changeAnimation(DIE_LEFT);
			else sprite->changeAnimation(DIE_RIGHT);
		}

		deathAngle += DEATH_ANGLE_STEP;
		deathTime += deltaTime;

		if (deathAngle >= 220 && !dead) {
			if (deathTime >= MAX_DEATH_TIME) {
				dead = true;
			}
		}

		if (!dead) {
			pos.y = int(startY - 50 * sin(3.14159 * deathAngle / 180.f));
			pos.x += 0.5;
		}
	}
	++rollTimer;
	sprite->setPosition(glm::vec2(int(tileMapDispl.x + pos.x), int(tileMapDispl.y + pos.y)));
}

void Armadillo::onEntityHit(bool isPlayer) {
	if ((player->isPlayerGroundPounding() || !isPlayer) && !rolling) {
		if (!dying) player->addScore(10);
		dying = true;
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
