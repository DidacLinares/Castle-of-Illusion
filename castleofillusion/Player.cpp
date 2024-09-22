#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define MAX_SPEED 2
#define ACCELERATION 0.0025


enum PlayerAnims {
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	bJumping = false;
	speed = 0;
	spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));
		
	sprite->changeAnimation(0);
	tileMapDispl = glm::vec2(tileMapPos);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime) {
	sprite->update(deltaTime);
	cout << speed << endl;
	if(Game::instance().getKey(GLFW_KEY_LEFT)) {
		if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
		speed -= ACCELERATION * deltaTime;
		if (speed <= -MAX_SPEED) speed = -MAX_SPEED;
		posPlayer.x += speed;
	}
	else if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
		if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
		speed += ACCELERATION * deltaTime;
		if (speed >= MAX_SPEED) speed = MAX_SPEED;
		posPlayer.x += speed;
	}
	else if (speed != 0) {
		if (sprite->animation() == MOVE_LEFT) {
			speed += ACCELERATION * deltaTime;
			if (speed > 0) speed = 0;
			posPlayer.x += speed;
		}
		else {
			speed -= ACCELERATION * deltaTime;
			if (speed < 0) speed = 0;
			posPlayer.x += speed;
		}
	}
	else {
		if (sprite->animation() == MOVE_LEFT) sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(STAND_RIGHT);
	}

	if (map->collisionMoveRight(posPlayer, glm::vec2(32, 32))) {
		posPlayer.x -= speed;
		sprite->changeAnimation(STAND_RIGHT);
		speed = 0;
	}
	if (map->collisionMoveLeft(posPlayer, glm::vec2(32, 32))) {
		posPlayer.x -= speed;
		sprite->changeAnimation(STAND_LEFT);
		speed = 0;
	}

	if (bJumping) {
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180) {
			bJumping = false;
			posPlayer.y = startY;
		}
		else {
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90) bJumping = !map->collisionMoveDown(posPlayer, glm::vec2(32, 32), &posPlayer.y);
		}
	}
	else {
		posPlayer.y += FALL_STEP;
		if (map->collisionMoveDown(posPlayer, glm::vec2(32, 32), &posPlayer.y)) {
			if (Game::instance().getKey(GLFW_KEY_UP)) {
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	
	sprite->setPosition(glm::vec2(int(tileMapDispl.x + posPlayer.x), int(tileMapDispl.y + posPlayer.y)));
}

void Player::render() {
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap) {
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos) {
	posPlayer = pos;
	sprite->setPosition(glm::vec2(tileMapDispl.x + posPlayer.x, tileMapDispl.y + posPlayer.y));
}




