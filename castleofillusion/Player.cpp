#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_STEP 3
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define MAX_SPEED 3
#define ACCELERATION 0.01f
#define GRAVITY 0.0075

#define MAX_RISE_TIME 200
#define MAX_DEATH_TIME 3050
#define DEATH_ANGLE_STEP 4

#define OFFSET_X 0.0625
#define OFFSET_Y 0.2

#define HITBOX_X 20
#define HITBOX_Y 32

#define BLINK_TIME 200

enum PlayerAnims {
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, CROUCH_LEFT, CROUCH_RIGHT, GROUND_POUND_LEFT, GROUND_POUND_RIGHT, JUMP_LEFT, JUMP_RIGHT, FALL_LEFT, FALL_RIGHT, BREAKING_LEFT, BREAKING_RIGHT, BALANCE_LEFT, BALANCE_RIGHT,
	PREPARED_TO_GRAB_LEFT, PREPARED_TO_GRAB_RIGHT, GRABBING_LEFT, GRABBING_RIGHT, STAND_GRABBED_LEFT, STAND_GRABBED_RIGHT, MOVE_GRABBED_LEFT, MOVE_GRABBED_RIGHT, JUMP_GRABBED_LEFT, JUMP_GRABBED_RIGHT, FALL_GRABBED_LEFT, FALL_GRABBED_RIGHT, DIE_LEFT, DIE_RIGHT
};

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	bJumping = false;
	falling = false;
	crouching = false;
	groundpounding = false;
	hit = false;
	shortenedJump = false;
	speedX = 0;
	lives = 3;
	spritesheet.loadFromFile("images/mickey.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(25, 38), glm::vec2(OFFSET_X, OFFSET_Y), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(30);

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

	sprite->setAnimationSpeed(BALANCE_LEFT, 8);
	sprite->addKeyframe(BALANCE_LEFT, glm::vec2(15 * OFFSET_X, 0.f));
	sprite->addKeyframe(BALANCE_LEFT, glm::vec2(15 * OFFSET_X, OFFSET_Y));

	sprite->setAnimationSpeed(BALANCE_RIGHT, 8);
	sprite->addKeyframe(BALANCE_RIGHT, glm::vec2(15 * OFFSET_X, 0.f));
	sprite->addKeyframe(BALANCE_RIGHT, glm::vec2(15 * OFFSET_X, OFFSET_Y));

	sprite->setAnimationSpeed(PREPARED_TO_GRAB_LEFT, 1);
	sprite->addKeyframe(PREPARED_TO_GRAB_LEFT, glm::vec2(0.f, 2*OFFSET_Y));

	sprite->setAnimationSpeed(PREPARED_TO_GRAB_RIGHT, 1);
	sprite->addKeyframe(PREPARED_TO_GRAB_RIGHT, glm::vec2(0.f, 2*OFFSET_Y));

	sprite->setAnimationSpeed(GRABBING_LEFT, 1);
	sprite->addKeyframe(GRABBING_RIGHT, glm::vec2(4 * OFFSET_X, 2 * OFFSET_Y));

	sprite->setAnimationSpeed(GRABBING_RIGHT, 1);
	sprite->addKeyframe(GRABBING_RIGHT, glm::vec2(4 * OFFSET_X, 2 * OFFSET_Y));

	sprite->setAnimationSpeed(STAND_GRABBED_LEFT, 1);
	sprite->addKeyframe(STAND_GRABBED_LEFT, glm::vec2(4 *OFFSET_X, 2 * OFFSET_Y));

	sprite->setAnimationSpeed(STAND_GRABBED_RIGHT, 1);
	sprite->addKeyframe(STAND_GRABBED_RIGHT, glm::vec2(4 * OFFSET_X, 2 * OFFSET_Y));

	sprite->setAnimationSpeed(MOVE_GRABBED_LEFT, 6);
	sprite->addKeyframe(MOVE_GRABBED_LEFT, glm::vec2(0, 3 * OFFSET_Y));
	sprite->addKeyframe(MOVE_GRABBED_LEFT, glm::vec2(1 * OFFSET_X, 3 * OFFSET_Y));
	sprite->addKeyframe(MOVE_GRABBED_LEFT, glm::vec2(2 * OFFSET_X, 3 * OFFSET_Y));
	sprite->addKeyframe(MOVE_GRABBED_LEFT, glm::vec2(3 * OFFSET_X, 3 * OFFSET_Y));
	sprite->addKeyframe(MOVE_GRABBED_LEFT, glm::vec2(4 * OFFSET_X, 3 * OFFSET_Y));
	sprite->addKeyframe(MOVE_GRABBED_LEFT, glm::vec2(5 * OFFSET_X, 3 * OFFSET_Y));

	sprite->setAnimationSpeed(MOVE_GRABBED_RIGHT, 6);
	sprite->addKeyframe(MOVE_GRABBED_RIGHT, glm::vec2(0, 3 * OFFSET_Y));
	sprite->addKeyframe(MOVE_GRABBED_RIGHT, glm::vec2(1 * OFFSET_X, 3 * OFFSET_Y));
	sprite->addKeyframe(MOVE_GRABBED_RIGHT, glm::vec2(2 * OFFSET_X, 3 * OFFSET_Y));
	sprite->addKeyframe(MOVE_GRABBED_RIGHT, glm::vec2(3 * OFFSET_X, 3 * OFFSET_Y));
	sprite->addKeyframe(MOVE_GRABBED_RIGHT, glm::vec2(4 * OFFSET_X, 3 * OFFSET_Y));
	sprite->addKeyframe(MOVE_GRABBED_RIGHT, glm::vec2(5 * OFFSET_X, 3 * OFFSET_Y));

	sprite->setAnimationSpeed(JUMP_GRABBED_LEFT, 1);
	sprite->addKeyframe(JUMP_GRABBED_LEFT, glm::vec2(7 * OFFSET_X, 3 * OFFSET_Y));

	sprite->setAnimationSpeed(JUMP_GRABBED_RIGHT, 1);
	sprite->addKeyframe(JUMP_GRABBED_RIGHT, glm::vec2(7 * OFFSET_X, 3 * OFFSET_Y));

	sprite->setAnimationSpeed(FALL_GRABBED_LEFT, 1);
	sprite->addKeyframe(FALL_GRABBED_LEFT, glm::vec2(8 * OFFSET_X, 3 * OFFSET_Y));

	sprite->setAnimationSpeed(FALL_GRABBED_RIGHT, 1);
	sprite->addKeyframe(FALL_GRABBED_RIGHT, glm::vec2(8 * OFFSET_X, 3 * OFFSET_Y));

	sprite->setAnimationSpeed(DIE_LEFT, 4);
	sprite->addKeyframe(DIE_LEFT, glm::vec2(12 * OFFSET_X, 3 * OFFSET_Y));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(13 * OFFSET_X, 3 * OFFSET_Y));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(11 * OFFSET_X, 2 * OFFSET_Y));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(12 * OFFSET_X, 2 * OFFSET_Y));


	sprite->setAnimationSpeed(DIE_RIGHT, 4);
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(12 * OFFSET_X, 3 * OFFSET_Y));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(13 * OFFSET_X, 3 * OFFSET_Y));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(11 * OFFSET_X, 2 * OFFSET_Y));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(12 * OFFSET_X, 2 * OFFSET_Y));

	changeAnim(STAND_RIGHT);
	tileMapDispl = glm::vec2(tileMapPos);
	hitbox_x = 20;
	hitbox_y = 32;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	
}

bool Player::movingLeft() {
	return sprite->animation() % 2 == 0;
}

void Player::update(int deltaTime) {
	sprite->update(deltaTime);
	if (!dying) {
		if (Game::instance().getKey(GLFW_KEY_G)) {
			setGodMode(!isGodMode());
			cout << "God mode: " << isGodMode() << endl;
			Game::instance().keyReleased(GLFW_KEY_G);
		}
		if (Game::instance().getKey(GLFW_KEY_H)) {
			setLives(3);
			cout << "Lives: " << getLives() << endl;
			Game::instance().keyReleased(GLFW_KEY_H);
		}
		crouching = false;
		if (Game::instance().getKey(GLFW_KEY_S) && !object) {
			if (falling) {
				if (movingLeft()) {
					changeAnim(GROUND_POUND_LEFT);
				}
				else {
					changeAnim(GROUND_POUND_RIGHT);
				}
				groundpounding = true;
			}
			else if (!bJumping) {
				if (movingLeft()) {
					changeAnim(CROUCH_LEFT);
				}
				else {
					changeAnim(CROUCH_RIGHT);
				}
				crouching = true;
			}
		}

		if (Game::instance().getKey(GLFW_KEY_A) && Game::instance().getKey(GLFW_KEY_D) && !crouching) {
			if (movingLeft()) {
				if (!bJumping || !falling) changeAnim(BREAKING_LEFT);
				speedX += ACCELERATION * deltaTime;
				if (speedX > 0) {
					speedX = 0;
					if(sprite->animation() != STAND_LEFT) changeAnim(STAND_LEFT);
				}
				pos.x += speedX;
			}
			else {
				if (!bJumping || !falling) changeAnim(BREAKING_RIGHT);
				speedX -= ACCELERATION * deltaTime;
				if (speedX < 0) {
					speedX = 0;
					if(sprite->animation() != STAND_RIGHT) changeAnim(STAND_RIGHT);

				}
				pos.x += speedX;
			}
		}
		else if (Game::instance().getKey(GLFW_KEY_A) && !crouching) {
			if (object && sprite->animation() != MOVE_GRABBED_LEFT) changeAnim(MOVE_GRABBED_LEFT);
			else if (sprite->animation() != MOVE_LEFT && sprite->animation() != GROUND_POUND_LEFT && sprite->animation() != JUMP_LEFT && sprite->animation() != FALL_LEFT && !groundpounding) changeAnim(MOVE_LEFT);
			speedX -= ACCELERATION * deltaTime;
			if (speedX <= -MAX_SPEED) speedX = -MAX_SPEED;
			pos.x += speedX;
		}
		else if (Game::instance().getKey(GLFW_KEY_D) && !crouching) {
			if (object && sprite->animation() != MOVE_GRABBED_RIGHT) changeAnim(MOVE_GRABBED_RIGHT);
			else if (sprite->animation() != MOVE_RIGHT && sprite->animation() != GROUND_POUND_RIGHT && sprite->animation() != JUMP_RIGHT && sprite->animation() != FALL_RIGHT && !groundpounding) changeAnim(MOVE_RIGHT);
			speedX += ACCELERATION * deltaTime;
			if (speedX >= MAX_SPEED) speedX = MAX_SPEED;
			pos.x += speedX;
		}
		else if (speedX != 0) {
			if (movingLeft()) {
				if (!bJumping && !falling && !groundpounding && !crouching) changeAnim(BREAKING_LEFT);
				speedX += ACCELERATION * deltaTime;
				if (speedX > 0) {
					speedX = 0;
					if (!bJumping && !falling && !groundpounding && !crouching) changeAnim(STAND_LEFT);
				}
				pos.x += speedX;
			}
			else {
				if (!bJumping && !falling && !groundpounding && !crouching) changeAnim(BREAKING_RIGHT);
				speedX -= ACCELERATION * deltaTime;
				if (speedX < 0) {
					speedX = 0;
					if (!bJumping && !falling && !groundpounding && !crouching) changeAnim(STAND_RIGHT);
				}
				pos.x += speedX;
			}
		}
		else {
			if ((sprite->animation() == MOVE_LEFT || sprite->animation() == CROUCH_LEFT || sprite->animation() == BREAKING_LEFT) && !crouching && !groundpounding) changeAnim(STAND_LEFT);
			else if ((sprite->animation() == MOVE_RIGHT || sprite->animation() == CROUCH_RIGHT || sprite->animation() == BREAKING_RIGHT) && !crouching && !groundpounding) changeAnim(STAND_RIGHT);
		}

		if (map->collisionMoveRight(pos, glm::vec2(hitbox_x, hitbox_y))) {
			//pos.x -= speedX;
			if (sprite->animation() != STAND_RIGHT) changeAnim(STAND_RIGHT);
			speedX = 0;
		}
		if (map->collisionMoveLeft(pos, glm::vec2(hitbox_x, hitbox_y))) {
			//pos.x -= speedX;
			if (sprite->animation() != STAND_LEFT) changeAnim(STAND_LEFT);
			speedX = 0;
		}
		vector<bool> raycast(3, false);
		map->raycastDown(pos, glm::vec2(hitbox_x, hitbox_y), raycast);
		if (sprite->animation() == STAND_LEFT && raycast[0] && !raycast[1] && !raycast[2]) { // 0 esquerra, 1 centre, 2 dreta
			changeAnim(BALANCE_LEFT);
		}
		if (sprite->animation() == STAND_RIGHT && !raycast[0] && !raycast[1] && raycast[2]) {
			changeAnim(BALANCE_RIGHT);
		}

		if (bJumping) {
			jumpAngle += JUMP_ANGLE_STEP;
			if (!Game::instance().getKey(GLFW_KEY_W) && !falling && startY - pos.y > 40 && jumpAngle < 90) {
				falling = true;
				shortenedJump = true;
				maxY = startY - pos.y;
				jumpAngle = 92;
			}
			//if (groundpounding) jumpAngle += 1;
			if (jumpAngle == 180) {
				bJumping = false;
			}
			else {
				float antY = pos.y;
				if (!shortenedJump) pos.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
				else pos.y = int(startY - maxY * sin(3.14159f * jumpAngle / 180.f)); 
				bool collision = false;
				if (jumpAngle > 90) {
					if (pos.y - antY > 4) {
						float finalPos = pos.y;
						pos.y = antY;
						if (pos.y < finalPos) { //evitem bucles infinits
							while (pos.y < finalPos && !collision) {
								pos.y += 1;
								collision = map->collisionMoveDown(pos, glm::vec2(hitbox_x, hitbox_y), &pos.y);
							}
						}
					}
					if (collision) bJumping = collision;
					else bJumping = !map->collisionMoveDown(pos, glm::vec2(hitbox_x, hitbox_y), &pos.y);
					falling = true;
					if (!groundpounding && sprite->animation() != GROUND_POUND_LEFT && movingLeft()) {
						if (object) {
							changeAnim(FALL_GRABBED_LEFT);
						}
						else {
							changeAnim(FALL_LEFT);
						}
					}
					else if (!groundpounding && sprite->animation() != GROUND_POUND_RIGHT) {
						if (object) {
							changeAnim(FALL_GRABBED_RIGHT);
						}
						else {
							changeAnim(FALL_RIGHT);
						}
					}
				}
			}
		}
		else {
			pos.y += FALL_STEP;
			checkGroundCollision();
		}

		if (invulnerable) {
			invulnerableTimeLeft -= deltaTime;
			if (invulnerableTimeLeft <= 0.0f) {
				invulnerable = false;
			}
		}
		if (object) {
			switch (sprite->animation()) {
			case STAND_LEFT:
				changeAnim(STAND_GRABBED_LEFT);
				break;
			case STAND_RIGHT:
				changeAnim(STAND_GRABBED_RIGHT);
				break;
			default:
				break;
			}
		}
		if (lives <= 0) {
			dying = true;
		}
	}
	else {
		if (firstDyingIteration) {
			Game::instance().getScene()->changeMusicToDying();
			firstDyingIteration = false;
		}
		if (sprite->animation() != DIE_RIGHT && sprite->animation() != DIE_LEFT) {
			startX = pos.x;
			startY = pos.y;

			if (movingLeft()) sprite->changeAnimation(DIE_LEFT);
			else sprite->changeAnimation(DIE_RIGHT);
		}

		deathAngle += DEATH_ANGLE_STEP;
		deathTime += deltaTime;

		if (deathAngle >= 220 && !dead) {
			musicDeathfase = true;
			if (deathTime >= MAX_DEATH_TIME) {
				dead = true;
				Game::instance().getScene()->changeScene();
			}
		}

		if (!dead && !musicDeathfase) {
			pos.y = int(startY - 50 * sin(3.14159 * deathAngle / 180.f));
			pos.x += 0.5;
		}
		else {
			pos.y += 4;
		}
	}
	if (pos.y >= 240) dying = true;
	sprite->setPosition(glm::vec2(int(tileMapDispl.x + pos.x), int(tileMapDispl.y + pos.y)));
}

void Player::checkGroundCollision() {
	if (map->collisionMoveDown(pos, glm::vec2(hitbox_x, hitbox_y),&pos.y)) {
		falling = false;
		groundpounding = false;
		shortenedJump = false;
		if (sprite->animation() == GROUND_POUND_LEFT || sprite->animation() == JUMP_LEFT || sprite->animation() == FALL_LEFT) changeAnim(STAND_LEFT);
		else if (sprite->animation() == GROUND_POUND_RIGHT || sprite->animation() == JUMP_RIGHT || sprite->animation() == FALL_RIGHT) changeAnim(STAND_RIGHT);
		else if (sprite->animation() == JUMP_GRABBED_LEFT || sprite->animation() == FALL_GRABBED_LEFT) changeAnim(STAND_GRABBED_LEFT);
		else if (sprite->animation() == JUMP_GRABBED_RIGHT || sprite->animation() == FALL_GRABBED_RIGHT) changeAnim(STAND_GRABBED_RIGHT);
		if (Game::instance().getKey(GLFW_KEY_W)) {
			if (sprite->animation() == MOVE_LEFT || sprite->animation() == CROUCH_LEFT || sprite->animation() == STAND_LEFT || sprite->animation() == GROUND_POUND_RIGHT) {
				if (object) {
					changeAnim(JUMP_GRABBED_LEFT);
				}
				else {
					changeAnim(JUMP_LEFT);
				}	
			}
			else {
				if (object) {
					changeAnim(JUMP_GRABBED_RIGHT);
				}
				else {
					changeAnim(JUMP_RIGHT);
				}
			}
			if (jumpSound && !soundEngine->isCurrentlyPlaying(jumpSound->getName())) {
				soundEngine->play2D(jumpSound);
			}
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

bool Player::isPlayerGroundPounding() {
	return groundpounding;
}

bool Player::isHit() {
	return hit;
}

void Player::onEntityHit(bool isPlayer) {
	if (isGodMode()) return;
	cout << "Lives before: " << lives<< endl;

	--lives;
	if (lives < 0) {
		// Mort del Mickey
		cout << "Mickey is dead!" << endl;
		this->setDead(true);
		return;
	}
	cout << "Lives after: " << lives << endl;

	// Mickey needs to be invulnerable for a while
	this->setInvulnerable(true);
}

void Player::setGodMode(bool godMode) {
	this->godMode = godMode;
}

bool Player::isGodMode() {
	return godMode;
}

bool Player::checkCollision(glm::vec4 hitboxentity) {
	glm::vec4 hitbox = getCollisionBox();
	return (hitbox.x < hitboxentity.x + hitboxentity.z && // hitboxplayer.left < hitboxenemy.right
		hitbox.x + hitbox.z > hitboxentity.x && // hitboxplayer.right > hitboxenemy.left
		hitbox.y < hitboxentity.y + hitboxentity.w && // hitboxplayer.top < hitboxenemy.bottom
		hitbox.y + hitbox.w > hitboxentity.y);  // hitboxplayer.bottom > hitboxenemy.top
}

void Player::setSoundEngineAndSounds(irrklang::ISoundEngine* soundEngine, irrklang::ISoundSource* jumpSound) {
	this->soundEngine = soundEngine;
	this->jumpSound = jumpSound;
}

void Player::grabAnimation() {
	if (movingLeft()) changeAnim(PREPARED_TO_GRAB_LEFT);
	else changeAnim(PREPARED_TO_GRAB_RIGHT);
}

void Player::setObject(bool object) {
	this->object = object;

	if (movingLeft()) {
		changeAnim(STAND_GRABBED_LEFT);
	}
	else {
		changeAnim(STAND_GRABBED_RIGHT);
	}
}

bool Player::getObject() {
	return object;
}
bool Player::moving() {
	return speedX != 0 || bJumping || falling;
}

int Player::getLives() {
	return lives;
}

int Player::getScore() {
	return score;
}

void Player::addScore(int points) {
	score += points;
}

void Player::changeAnim(int anim) {
	if (object) {
		if (anim != STAND_LEFT && anim != STAND_RIGHT && anim != STAND_GRABBED_LEFT && anim != STAND_GRABBED_RIGHT && anim != MOVE_GRABBED_LEFT && anim != MOVE_GRABBED_RIGHT && 
			anim != PREPARED_TO_GRAB_LEFT && anim != PREPARED_TO_GRAB_RIGHT && anim != GRABBING_LEFT && anim != GRABBING_RIGHT && anim != JUMP_GRABBED_LEFT &&
			anim != JUMP_GRABBED_RIGHT && anim != FALL_GRABBED_LEFT && anim != FALL_GRABBED_RIGHT) {
			return;
		}
	}

	sprite->changeAnimation(anim);
}