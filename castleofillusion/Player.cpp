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
#define ACCELERATION 0.0075f
#define GRAVITY 0.0075

#define OFFSET_X 0.066
#define OFFSET_Y 0.1

#define HITBOX_X 20
#define HITBOX_Y 32

#define BLINK_TIME 200

enum PlayerAnims {
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, CROUCH_LEFT, CROUCH_RIGHT, GROUND_POUND_LEFT, GROUND_POUND_RIGHT, JUMP_LEFT, JUMP_RIGHT, FALL_LEFT, FALL_RIGHT, BREAKING_LEFT, BREAKING_RIGHT
};


void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	bJumping = false;
	falling = false;
	crouching = false;
	groundpounding = false;
	hit = false;
	speedX = 0;

	spritesheet.loadFromFile("images/mickey.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(OFFSET_X, OFFSET_X), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(14);

	sprite->setAnimationSpeed(STAND_LEFT, 4);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(STAND_LEFT, glm::vec2(OFFSET_X, 0.0f));

	sprite->setAnimationSpeed(STAND_RIGHT, 4);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(OFFSET_X, 0.0f));

	sprite->setAnimationSpeed(CROUCH_LEFT, 8);
	sprite->addKeyframe(CROUCH_LEFT, glm::vec2(0.f, OFFSET_Y));

	sprite->setAnimationSpeed(CROUCH_RIGHT, 8);
	sprite->addKeyframe(CROUCH_RIGHT, glm::vec2(0.f, OFFSET_Y));

	sprite->setAnimationSpeed(GROUND_POUND_LEFT, 8);
	sprite->addKeyframe(GROUND_POUND_LEFT, glm::vec2(6 * OFFSET_X, OFFSET_Y));

	sprite->setAnimationSpeed(GROUND_POUND_RIGHT, 8);
	sprite->addKeyframe(GROUND_POUND_RIGHT, glm::vec2(6 * OFFSET_X, OFFSET_Y));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(2 * OFFSET_X, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * OFFSET_X, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(4 * OFFSET_X, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(5 * OFFSET_X, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(6 * OFFSET_X, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(7 * OFFSET_X, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(8 * OFFSET_X, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(9 * OFFSET_X, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(6 * OFFSET_X, 0.f));
		
	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * OFFSET_X, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(3 * OFFSET_X, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(4 * OFFSET_X, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(5 * OFFSET_X, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(6 * OFFSET_X, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(7 * OFFSET_X, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(8 * OFFSET_X, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(9 * OFFSET_X, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(6 * OFFSET_X, 0.f));

	sprite->setAnimationSpeed(JUMP_LEFT, 1);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(2 * OFFSET_X, OFFSET_Y));

	sprite->setAnimationSpeed(JUMP_RIGHT, 1);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(2 * OFFSET_X, OFFSET_Y));

	sprite->setAnimationSpeed(BREAKING_LEFT, 1);
	sprite->addKeyframe(BREAKING_LEFT, glm::vec2(10 * OFFSET_X, 0.f));

	sprite->setAnimationSpeed(BREAKING_RIGHT, 1);
	sprite->addKeyframe(BREAKING_RIGHT, glm::vec2(10 * OFFSET_X, 0.f));

	sprite->setAnimationSpeed(FALL_LEFT, 1);
	sprite->addKeyframe(FALL_LEFT, glm::vec2(3 * OFFSET_X, OFFSET_Y));
	
	sprite->setAnimationSpeed(FALL_RIGHT, 1);
	sprite->addKeyframe(FALL_RIGHT, glm::vec2(3 * OFFSET_X, OFFSET_Y));


		
	sprite->changeAnimation(STAND_RIGHT);
	tileMapDispl = glm::vec2(tileMapPos);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	
}

bool Player::movingLeft() {
	return sprite->animation() % 2 == 0;
}

void Player::update(int deltaTime) {
	sprite->update(deltaTime);
	crouching = false;
	if (Game::instance().getKey(GLFW_KEY_S)) {
		if (bJumping || falling) {
			if (movingLeft()) {
				sprite->changeAnimation(GROUND_POUND_LEFT);
			}
			else {
				sprite->changeAnimation(GROUND_POUND_RIGHT);
			}
			groundpounding = true;
		}
		else {
			if (movingLeft()) {
				sprite->changeAnimation(CROUCH_LEFT);
			}
			else {
				sprite->changeAnimation(CROUCH_RIGHT);
			}
			crouching = true;
		}
	}

	if (Game::instance().getKey(GLFW_KEY_A) && Game::instance().getKey(GLFW_KEY_D) && !crouching) {
		if (movingLeft()) {
			if (!bJumping || !falling) sprite->changeAnimation(BREAKING_LEFT);
			speedX += ACCELERATION * deltaTime;
			if (speedX > 0) {
				speedX = 0;
				sprite->changeAnimation(STAND_LEFT);
			}
			pos.x += speedX;
		}
		else {
			if (!bJumping || !falling) sprite->changeAnimation(BREAKING_RIGHT);
			speedX -= ACCELERATION * deltaTime;
			if (speedX < 0) {
				speedX = 0;
				sprite->changeAnimation(STAND_RIGHT);

			}
			pos.x += speedX;
		}
	}
	else if(Game::instance().getKey(GLFW_KEY_A) && !crouching) {
		if (sprite->animation() != MOVE_LEFT && sprite->animation() != GROUND_POUND_LEFT && sprite->animation() != JUMP_LEFT && sprite->animation() != FALL_LEFT && !groundpounding) sprite->changeAnimation(MOVE_LEFT);
		speedX -= ACCELERATION * deltaTime;
		if (speedX <= -MAX_SPEED) speedX = -MAX_SPEED;
		pos.x += speedX;
	}
	else if (Game::instance().getKey(GLFW_KEY_D) && !crouching) {
		if (sprite->animation() != MOVE_RIGHT && sprite->animation() != GROUND_POUND_RIGHT && sprite->animation() != JUMP_RIGHT && sprite->animation() != FALL_RIGHT && !groundpounding) sprite->changeAnimation(MOVE_RIGHT);
		speedX += ACCELERATION * deltaTime;
		if (speedX >= MAX_SPEED) speedX = MAX_SPEED;
		pos.x += speedX;
	}
	else if (speedX != 0) {
		if (movingLeft()) {
			if (!bJumping && !falling && !groundpounding && !crouching) sprite->changeAnimation(BREAKING_LEFT);
			speedX += ACCELERATION * deltaTime;
			if (speedX > 0) {
				speedX = 0;
				if (!bJumping && !falling && !groundpounding && !crouching) sprite->changeAnimation(STAND_LEFT);
			}
			pos.x += speedX;
		}
		else {
			if (!bJumping && !falling && !groundpounding && !crouching) sprite->changeAnimation(BREAKING_RIGHT);
			speedX -= ACCELERATION * deltaTime;
			if (speedX < 0) {
				speedX = 0;
				if (!bJumping && !falling && !groundpounding && !crouching) sprite->changeAnimation(STAND_RIGHT);
			}
			pos.x += speedX;
		}
	}
	else {
		if (sprite->animation() == MOVE_LEFT || sprite->animation() == CROUCH_LEFT  && !crouching && !groundpounding) sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == CROUCH_RIGHT && !crouching && !groundpounding) sprite->changeAnimation(STAND_RIGHT);
	}

	if (map->collisionMoveRight(pos, glm::vec2(HITBOX_X, HITBOX_Y))) {
		pos.x -= speedX;
		sprite->changeAnimation(STAND_RIGHT);
		speedX = 0;
	}
	if (map->collisionMoveLeft(pos, glm::vec2(HITBOX_X, HITBOX_Y))) {
		pos.x -= speedX;
		sprite->changeAnimation(STAND_LEFT);
		speedX = 0;
	}
	/*
	if (Game::instance().getKey(GLFW_KEY_W) && !bJumping && !falling) {
		startY = pos.y;
		speedY = JUMP_STEP;
		pos.y -= speedY;
		bJumping = true;
	}
	else if (Game::instance().getKey(GLFW_KEY_W) && bJumping) {
		speedY -= GRAVITY * deltaTime;
		pos.y -= speedY;
		if (speedY <= 0) {
			falling = true;
			bJumping = false;
		}
		if (!map->collisionMoveDown(pos, glm::vec2(32, 32), &pos.y)) pos.y -= (JUMP_STEP - GRAVITY * deltaTime);
	}
	else if (bJumping && !Game::instance().getKey(GLFW_KEY_W)) {
		bJumping = false;
		falling = true;
		speedY = 0.1f;
	}
	else if (falling) {
		if(speedY < FALL_STEP) speedY += GRAVITY * deltaTime;
		pos.y += speedY;
		if (map->collisionMoveDown(pos, glm::vec2(32, 32), &pos.y)) falling = false;
	}
	else if (!bJumping && !falling) {
		speedY = GRAVITY * deltaTime;
		pos.y += speedY;
		if(!map->collisionMoveDown(pos, glm::vec2(32, 32), &pos.y)) falling = true;
	}*/
		
	if (bJumping) {
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180) {
			bJumping = false;
			pos.y = startY;
		}
		else {
			pos.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90) {
				bJumping = !map->collisionMoveDown(pos, glm::vec2(HITBOX_X, HITBOX_Y), &pos.y);
				if (!groundpounding && sprite->animation() != GROUND_POUND_LEFT && movingLeft()) sprite->changeAnimation(FALL_LEFT);
				else if (!groundpounding && sprite->animation() != GROUND_POUND_RIGHT) sprite->changeAnimation(FALL_RIGHT);
			}
		}
	}
	else {
		pos.y += FALL_STEP;
		checkGroundCollision();
		if (groundpounding) {
			pos.y += FALL_STEP;
			checkGroundCollision();
			pos.y += FALL_STEP;
			checkGroundCollision();
		}
	}

	if (invulnerable) {
		invulnerableTimeLeft -= deltaTime;
		cout << "Invulnerable time left: " << invulnerableTimeLeft << endl;
		cout << "Delta time: " << deltaTime << endl;
		if (invulnerableTimeLeft <= 0.0f) {
			invulnerable = false;
		}
	}
	sprite->setPosition(glm::vec2(int(tileMapDispl.x + pos.x), int(tileMapDispl.y + pos.y)));
}

void Player::checkGroundCollision() {
	if (map->collisionMoveDown(pos, glm::vec2(HITBOX_X, HITBOX_Y), &pos.y)) {
		falling = false;
		groundpounding = false;
		if (sprite->animation() == GROUND_POUND_LEFT || sprite->animation() == JUMP_LEFT || sprite->animation() == FALL_LEFT) sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == GROUND_POUND_RIGHT || sprite->animation() == JUMP_RIGHT || sprite->animation() == FALL_RIGHT) sprite->changeAnimation(STAND_RIGHT);
		if (Game::instance().getKey(GLFW_KEY_W)) {
			if (sprite->animation() == MOVE_LEFT || sprite->animation() == CROUCH_LEFT || sprite->animation() == STAND_LEFT || sprite->animation() == GROUND_POUND_RIGHT) sprite->changeAnimation(JUMP_LEFT);
			else sprite->changeAnimation(JUMP_RIGHT);
			bJumping = true;
			jumpAngle = 0;
			startY = pos.y;
		}
	}
}

void Player::render() {
	if (isInvulnerable()) {
		if (fmod(invulnerableTimeLeft, BLINK_TIME * 2) < BLINK_TIME) {
			return;
		}
	}

	sprite->render();
}

glm::vec4 Player::getCollisionBox() {
	return glm::vec4(pos.x, pos.y, HITBOX_X, HITBOX_Y);
}

bool Player::isPlayerGroundPounding() {
	return groundpounding;
}

bool Player::isHit() {
	return hit;
}

void Player::onEntityHit() {
	--lives;
	if (lives <= 0) {
		// Mort del Mickey
		cout << "Mickey is dead!" << endl;
		this->setDead(true);
		return;
	}

	// Mickey needs to be invulnerable for a while
	this->setInvulnerable(true);
}

