#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "FlowerEnemy.h"
#include "Game.h"
#include "FlowerProjectile.h"

#define OFFSET_X 0.20
#define OFFSET_Y 0

#define HITBOX_X 24
#define HITBOX_Y 32
#define MAX_RISE_TIME 200
#define MAX_DEATH_TIME 1000

#define PREPARE_ATTACK_RADIUS 150
#define ATTACK_RADIUS 100
#define ATTACK_COOLDOWN 3000
#define DEATH_ANGLE_STEP 4


enum FlowerAnims {
	STAND, DUMMY, PREPARING_ATTACK, DUMMY2, ATTACK, DIE
};


void FlowerEnemy::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	dead = false;
	timeSinceLastAttack = ATTACK_COOLDOWN;
	spritesheet.loadFromFile("images/FlowerEnemy.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 24), glm::vec2(OFFSET_X, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);

	sprite->setAnimationSpeed(DUMMY, 1);
	sprite->addKeyframe(DUMMY, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(DUMMY2, 1);
	sprite->addKeyframe(DUMMY2, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STAND, 1);
	sprite->addKeyframe(STAND, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(PREPARING_ATTACK, 1);
	sprite->addKeyframe(PREPARING_ATTACK, glm::vec2(OFFSET_X, 0.f));

	sprite->setAnimationSpeed(ATTACK, 1);
	sprite->addKeyframe(ATTACK, glm::vec2(2 * OFFSET_X, 0.f));

	sprite->setAnimationSpeed(DIE, 1);
	sprite->addKeyframe(DIE, glm::vec2(4 * OFFSET_X, 0.f));

	sprite->changeAnimation(0);

	hitbox_x = 16;
	hitbox_y = 24;

	pos.x = 0;
	pos.y = 0;

	tileMapDispl = glm::vec2(tileMapPos);
	sprite->setPosition(glm::vec2(pos.x, pos.y));

}

void FlowerEnemy::update(int deltaTime) {
	sprite->update(deltaTime);

	if (!dying) {
		pos.y += 4;
		map->collisionMoveDown(pos, glm::vec2(hitbox_x, hitbox_y), &pos.y);
		// Obtener la posición del jugador
		glm::vec2 playerPos = player->getPosition();

		// Calcular la distancia entre el enemigo y el jugador
		float distanceToPlayer = glm::length(playerPos - pos);

		timeSinceLastAttack += deltaTime;

		if (distanceToPlayer < PREPARE_ATTACK_RADIUS && !attacking) {
			if (sprite->animation() != PREPARING_ATTACK) {
				sprite->changeAnimation(PREPARING_ATTACK);
			}
		}
		else {
			if (sprite->animation() != STAND) {
				sprite->changeAnimation(STAND);
			}
		}
		
		// Si el jugador está dentro del radio de detección, atacar
		if (distanceToPlayer < ATTACK_RADIUS && timeSinceLastAttack >= ATTACK_COOLDOWN) {
			attacking = true;
			if (sprite->animation() != ATTACK) {
				sprite->changeAnimation(ATTACK);
			}

			timeSinceLastAttack = 0;

			// Crear proyectil
			for (int i = 0; i < 2; ++i) {
				FlowerProjectile* projectile = new FlowerProjectile();
				projectile->init(glm::ivec2(tileMapDispl), Game::instance().getScene()->getShaderProgram());
				projectile->setTileMap(map);
				projectile->setPlayer(player);
				// Position centered on top of the enemy
				projectile->setPosition(glm::vec2(int(pos.x - 4), int(pos.y - 12)));
				
				projectile->setDirection(i == 0);
				Game::instance().getScene()->addEntity(projectile);
			}
		}
		else {
			if (timeSinceLastAttack >= 1000 && attacking) {
				//sprite->changeAnimation(STAND);
				attacking = false;
			}
		}

		if (player->checkCollision(getCollisionBox())) {
			onEntityHit();
		}
	}
		
	else {
		if (sprite->animation() != DIE) {
			startX = pos.x;
			startY = pos.y;
			sprite->changeAnimation(DIE);
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

	sprite->setPosition(glm::vec2(int(tileMapDispl.x + pos.x), int(tileMapDispl.y + pos.y)));
}

void FlowerEnemy::onEntityHit(bool isPlayer) {
	if (player->isPlayerGroundPounding() || !isPlayer) {
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