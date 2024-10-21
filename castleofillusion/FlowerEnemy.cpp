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

#define DETECTION_RADIUS 50
#define ATTACK_COOLDOWN 3000
#define DEATH_ANGLE_STEP 4


enum FlowerAnims {
	STAND, ATTACK, DIE
};


void FlowerEnemy::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	dead = false;
	timeSinceLastAttack = ATTACK_COOLDOWN;
	spritesheet.loadFromFile("images/FlowerEnemy.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 24), glm::vec2(OFFSET_X, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(STAND, 1);
	sprite->addKeyframe(STAND, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(ATTACK, 2);
	sprite->addKeyframe(ATTACK, glm::vec2(OFFSET_X, 0.f));
	sprite->addKeyframe(ATTACK, glm::vec2(2 * OFFSET_X, 0.0f));

	sprite->setAnimationSpeed(DIE, 1);
	sprite->addKeyframe(DIE, glm::vec2(4 * OFFSET_X, 0.f));

	sprite->changeAnimation(0);
	pos.x = 0;
	pos.y = 0;

	tileMapDispl = glm::vec2(tileMapPos);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));

}

void FlowerEnemy::update(int deltaTime) {
	sprite->update(deltaTime);

	if (!dying) {
		// Obtener la posición del jugador
		glm::vec2 playerPos = player->getPosition();

		// Calcular la distancia entre el enemigo y el jugador
		float distanceToPlayer = glm::length(playerPos - pos);

		timeSinceLastAttack += deltaTime;
		
		// Si el jugador está dentro del radio de detección, atacar
		if (distanceToPlayer < DETECTION_RADIUS && timeSinceLastAttack >= ATTACK_COOLDOWN) {
			attacking = true;
			if (sprite->animation() != ATTACK) {
				sprite->changeAnimation(ATTACK);
			}

			timeSinceLastAttack = 0;
			// Aquí puedes añadir la lógica del ataque, como reducir la vida del jugador
		}
		else {
			// Si el jugador no está cerca, vuelve a la animación de estar quieto
			if (timeSinceLastAttack >= 1000 && attacking) {
				sprite->changeAnimation(STAND);
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

void FlowerEnemy::onEntityHit() {
	if (player->isPlayerGroundPounding()) {
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