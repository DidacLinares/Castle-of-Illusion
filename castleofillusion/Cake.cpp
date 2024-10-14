#include "Cake.h"
#include <iostream>

#define OFFSET_X 0.125
#define OFFSET_Y 0.5

#define HITBOX_X 16
#define HITBOX_Y 16

void Cake::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	dead = false;
	spritesheet.loadFromFile("images/items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(OFFSET_X, OFFSET_Y), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(0, 1);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(1, 1);
	sprite->addKeyframe(1, glm::vec2(0.f, 0.f));
	
	sprite->changeAnimation(1);
	tileMapDispl = glm::vec2(tileMapPos);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Cake::update(int deltaTime) {
	if (checkCollision()) {
		onEntityHit();
	}
}

void Cake::onEntityHit() {
	int playerLives = player->getLives();
	if (playerLives < 3) {
		player->setLives(++playerLives);
	}
	dead = true;
}

void Cake::setPlayer(Player* player) {
	this->player = player;
}

glm::vec4 Cake::getCollisionBox() {
	return glm::vec4(pos.x, pos.y, HITBOX_X, HITBOX_Y); //canviar hitbox a variable i fer aquesta funcio virtual?
}

bool Cake::checkCollision() {
	glm::vec4 hitboxplayer = player->getCollisionBox();
	glm::vec4 hitboxitem = getCollisionBox();
	return (hitboxplayer.x < hitboxitem.x + hitboxitem.z && // hitboxplayer.left < hitboxitem.right
		hitboxplayer.x + hitboxplayer.z > hitboxitem.x && // hitboxplayer.right > hitboxitem.left
		hitboxplayer.y < hitboxitem.y + hitboxitem.w && // hitboxplayer.top < hitboxitem.bottom
		hitboxplayer.y + hitboxplayer.w > hitboxitem.y);  // hitboxplayer.bottom > hitboxitem.top
}
