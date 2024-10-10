#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "TreeEnemy.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_STEP 3
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define MAX_SPEED 4
#define ACCELERATION 0.0075f
#define GRAVITY 0.0075

#define OFFSET_X 0.25
#define OFFSET_Y 0

#define HITBOX_X 24
#define HITBOX_Y 32

enum TreeAnims {
	MOVE_LEFT, MOVE_RIGHT, DIE_LEFT, DIE_RIGHT 
};


void TreeEnemy::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
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
	posEnemy.x = 0;
	posEnemy.y = 0;
	tileMapDispl = glm::vec2(tileMapPos);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
	
}

bool TreeEnemy::movingLeft() {
	return sprite->animation() % 2 == 0;
}

void TreeEnemy::update(int deltaTime) {
	sprite->update(deltaTime); 
	if (sprite->animation() == MOVE_LEFT) posEnemy.x -= 2;
	else posEnemy.x += 2;

	if (map->collisionMoveRight(posEnemy, glm::vec2(HITBOX_X, HITBOX_Y))) {
		posEnemy.x -= 2;
		sprite->changeAnimation(MOVE_LEFT);
	}
	if (map->collisionMoveLeft(posEnemy, glm::vec2(HITBOX_X, HITBOX_Y))) {
		posEnemy.x += 2;
		sprite->changeAnimation(MOVE_RIGHT);
	}
	if (checkCollision()) {
		if (player->IsPlayerGroundPounding()) {
			cout << "death!!!!" << endl;
		}
		else {
			cout << "collision!!!!" << endl; // Aqui es treu una vida al mickey
		}
	}
	sprite->setPosition(glm::vec2(int(tileMapDispl.x + posEnemy.x), int(tileMapDispl.y + posEnemy.y)));
	
}

void TreeEnemy::render() {
	sprite->render();
}

void TreeEnemy::setTileMap(TileMap *tileMap) {
	map = tileMap;
}

void TreeEnemy::setPosition(const glm::vec2 &pos) {
	posEnemy = pos;
	sprite->setPosition(glm::vec2(tileMapDispl.x + posEnemy.x, tileMapDispl.y + posEnemy.y));
}

glm::vec2 TreeEnemy::getPosition()
{
	return posEnemy;
}

bool TreeEnemy::checkCollision() {
	glm::vec4 hitboxplayer = player->getCollisionBox();
	glm::vec4 hitboxenemy = glm::vec4(posEnemy.x, posEnemy.y, HITBOX_X, HITBOX_Y);
	return (hitboxplayer.x < hitboxenemy.x + hitboxenemy.z && // hitboxplayer.left < hitboxenemy.right
		hitboxplayer.x + hitboxplayer.z > hitboxenemy.x && // hitboxplayer.right > hitboxenemy.left
		hitboxplayer.y < hitboxenemy.y + hitboxenemy.w && // hitboxplayer.top < hitboxenemy.bottom
		hitboxplayer.y + hitboxplayer.w > hitboxenemy.y);  // hitboxplayer.bottom > hitboxenemy.top
}

void TreeEnemy::setPlayer(Player *player) {
	this->player = player;
}


