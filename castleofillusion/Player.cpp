#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_STEP 3
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define MAX_SPEED 4
#define ACCELERATION 0.0075
#define GRAVITY 0.0075
#define OFFSET 0.066
#define HITBOX_X 20
#define HITBOX_Y 32

enum PlayerAnims {
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, CROUCH_LEFT, CROUCH_RIGHT, GROUND_POUND_LEFT, GROUND_POUND_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	bJumping = false;
	falling = false;
	crouching = false;
	speedX = 0;
	spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);
	spritesheet.loadFromFile("images/mickey.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(OFFSET, OFFSET), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);
	
		sprite->setAnimationSpeed(STAND_LEFT, 4);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(STAND_LEFT, glm::vec2(OFFSET, 0.0f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 4);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(OFFSET, 0.0f));
		
		sprite->setAnimationSpeed(CROUCH_LEFT, 8);
		sprite->addKeyframe(CROUCH_LEFT, glm::vec2(0.5f, 0.25f));
		sprite->setAnimationSpeed(CROUCH_RIGHT, 8);
		sprite->addKeyframe(CROUCH_RIGHT, glm::vec2(0.5f, 0.f));

		sprite->setAnimationSpeed(GROUND_POUND_LEFT, 8);
		sprite->addKeyframe(GROUND_POUND_LEFT, glm::vec2(0.0f, 0.75f));
		sprite->setAnimationSpeed(GROUND_POUND_RIGHT, 8);
		sprite->addKeyframe(GROUND_POUND_RIGHT, glm::vec2(0.75f, 0.75f));


		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(2 * OFFSET, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * OFFSET, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(4 * OFFSET, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(5 * OFFSET, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(6 * OFFSET, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(7 * OFFSET, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(8 * OFFSET, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(9 * OFFSET, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(6 * OFFSET, 0.f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * OFFSET, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(3 * OFFSET, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(4 * OFFSET, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(5 * OFFSET, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(6 * OFFSET, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(7 * OFFSET, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(8 * OFFSET, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(9 * OFFSET, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(6 * OFFSET, 0.f));
		
	sprite->changeAnimation(0);
	tileMapDispl = glm::vec2(tileMapPos);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime) {
	sprite->update(deltaTime);
	crouching = false;
	if (Game::instance().getKey(GLFW_KEY_S)) {
		if (bJumping || falling) {
			if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT || sprite->animation() == GROUND_POUND_LEFT) {
				sprite->changeAnimation(GROUND_POUND_LEFT);
			}
			else {
				sprite->changeAnimation(GROUND_POUND_RIGHT);
			}
		}
		else {
			if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT || sprite->animation() == CROUCH_LEFT || sprite->animation() == GROUND_POUND_LEFT) {
				sprite->changeAnimation(CROUCH_LEFT);
			}
			else {
				sprite->changeAnimation(CROUCH_RIGHT);
			}
			crouching = true;
		}
	}

	if (Game::instance().getKey(GLFW_KEY_A) && Game::instance().getKey(GLFW_KEY_D) && !crouching) {
		if (sprite->animation() == MOVE_LEFT || sprite->animation() == GROUND_POUND_LEFT) {
			speedX += ACCELERATION * deltaTime;
			if (speedX > 0) {
				speedX = 0;
				sprite->changeAnimation(STAND_LEFT);
			}
			posPlayer.x += speedX;
		}
		else {
			speedX -= ACCELERATION * deltaTime;
			if (speedX < 0) {
				speedX = 0;
				sprite->changeAnimation(STAND_RIGHT);

			}
			posPlayer.x += speedX;
		}
	}
	else if(Game::instance().getKey(GLFW_KEY_A) && !crouching) {
		if (sprite->animation() != MOVE_LEFT && sprite->animation() != GROUND_POUND_LEFT) sprite->changeAnimation(MOVE_LEFT);
		speedX -= ACCELERATION * deltaTime;
		if (speedX <= -MAX_SPEED) speedX = -MAX_SPEED;
		posPlayer.x += speedX;
	}
	else if (Game::instance().getKey(GLFW_KEY_D) && !crouching) {
		if (sprite->animation() != MOVE_RIGHT && sprite->animation() != GROUND_POUND_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
		speedX += ACCELERATION * deltaTime;
		if (speedX >= MAX_SPEED) speedX = MAX_SPEED;
		posPlayer.x += speedX;
	}
	else if (speedX != 0) {
		if (sprite->animation() == MOVE_LEFT || sprite->animation() == CROUCH_LEFT) {
			speedX += ACCELERATION * deltaTime;
			if (speedX > 0) speedX = 0;
			posPlayer.x += speedX;
		}
		else {
			speedX -= ACCELERATION * deltaTime;
			if (speedX < 0) speedX = 0;
			posPlayer.x += speedX;
		}
	}
	else {
		if (sprite->animation() == MOVE_LEFT || sprite->animation() == CROUCH_LEFT && !crouching) sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == CROUCH_RIGHT && !crouching) sprite->changeAnimation(STAND_RIGHT);
	}

	if (map->collisionMoveRight(posPlayer, glm::vec2(32, 32))) {
		posPlayer.x -= speedX;
		sprite->changeAnimation(STAND_RIGHT);
		speedX = 0;
	}
	if (map->collisionMoveLeft(posPlayer, glm::vec2(32, 32))) {
		posPlayer.x -= speedX;
		sprite->changeAnimation(STAND_LEFT);
		speedX = 0;
	}
	/*
	if (Game::instance().getKey(GLFW_KEY_W) && !bJumping && !falling) {
		startY = posPlayer.y;
		speedY = JUMP_STEP;
		posPlayer.y -= speedY;
		bJumping = true;
	}
	else if (Game::instance().getKey(GLFW_KEY_W) && bJumping) {
		speedY -= GRAVITY * deltaTime;
		posPlayer.y -= speedY;
		if (speedY <= 0) {
			falling = true;
			bJumping = false;
		}
		if (!map->collisionMoveDown(posPlayer, glm::vec2(32, 32), &posPlayer.y)) posPlayer.y -= (JUMP_STEP - GRAVITY * deltaTime);
	}
	else if (bJumping && !Game::instance().getKey(GLFW_KEY_W)) {
		bJumping = false;
		falling = true;
		speedY = 0.1f;
	}
	else if (falling) {
		if(speedY < FALL_STEP) speedY += GRAVITY * deltaTime;
		posPlayer.y += speedY;
		if (map->collisionMoveDown(posPlayer, glm::vec2(32, 32), &posPlayer.y)) falling = false;
	}
	else if (!bJumping && !falling) {
		speedY = GRAVITY * deltaTime;
		posPlayer.y += speedY;
		if(!map->collisionMoveDown(posPlayer, glm::vec2(32, 32), &posPlayer.y)) falling = true;
	}*/
		
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
			if (sprite->animation() == GROUND_POUND_LEFT) sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == GROUND_POUND_RIGHT) sprite->changeAnimation(STAND_RIGHT);
			if (Game::instance().getKey(GLFW_KEY_W)) {
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




