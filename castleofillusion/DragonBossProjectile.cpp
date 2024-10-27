#include "DragonBossProjectile.h"

#include "Game.h"
#include <iostream>

#define OFFSET_X 0.33
#define OFFSET_Y 0


enum ProjectileAnims {
	MOVING
};

void DragonBossProjectile::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	dead = false;
	spritesheet.loadFromFile("images/DragonBossProjectile.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(6, 12), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(MOVING, 8);
	sprite->addKeyframe(MOVING, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVING, glm::vec2(OFFSET_X, 0.f));
	sprite->addKeyframe(MOVING, glm::vec2(2 * OFFSET_X, 0.0f));

	hitbox_x = 6;
	hitbox_y = 12;

	sprite->changeAnimation(0);
	pos.x = 0;
	pos.y = 0;

	tileMapDispl = glm::vec2(tileMapPos);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void DragonBossProjectile::update(int deltaTime) {
	sprite->update(deltaTime);
	if (!dead) {
		pos.y += 1;

		if (player->checkCollision(getCollisionBox())) {
			onEntityHit();
		}

		if (!dead && map->collisionMoveDown(pos, glm::vec2(hitbox_x, hitbox_y), &pos.y)) {
			cout << "Collision Dragon" << endl;
			dead = true;
		}

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	}
}

void DragonBossProjectile::onEntityHit(bool isPlayer) {
	if (isPlayer) {
		if (player->isInvulnerable()) {
			return;
		}
		else {
			player->onEntityHit();
		}

		dead = true;
	}
}